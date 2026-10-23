/*
 * A148097  Number of walks within N^3 (the first octant of Z^3) starting at
 *          (0,0,0) and consisting of n steps taken from {(-1, 0, 1), (0, 0,
 *          1), (0, 1, -1), (1, -1, 0)}.
 * https://oeis.org/A148097
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, walk
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 20, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   packuv, unpackuv
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) is the number of walks within the non‑negative octant of Z³
 * starting at (0,0,0) with steps (-1,0,1),(0,0,1),(0,1,-1),(1,-1,0).
 * The sequence satisfies a linear recurrence with integer coefficients that
 * is computed once from the first 200 terms.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/packuv.h"
#include "../utils/unpackuv.h"

/* ---------------------------------------------------------------------- */
/*  Helper hash table (open addressing) to store state counts            */
/* ---------------------------------------------------------------------- */
typedef struct {
    uint32_t *keys;          /* packed (x,y,z)                     */
    seq_t    *vals;          /* count of walks to that state       */
    bool     *used;          /* slot occupied?                      */
    size_t    size;          /* number of slots (power of two)     */
    size_t    mask;          /* size-1 (for modulo)                */
} HashTable;

static void ht_init(HashTable *ht, size_t power_of_two) {
    size_t sz = 1U << power_of_two;            /* 2^k slots, k >= 21 (~2M slots) */
    ht->size = sz;
    ht->mask  = sz - 1;
    ht->keys  = (uint32_t *)malloc(sz * sizeof(uint32_t));
    ht->vals  = (seq_t   *)malloc(sz * sizeof(seq_t   ));
    ht->used  = (bool    *)malloc(sz * sizeof(bool    ));
    for (size_t i = 0; i < sz; ++i) ht->used[i] = false;
}

static void ht_clear(HashTable *ht) {
    for (size_t i = 0; i < ht->size; ++i) ht->used[i] = false;
}

static inline void ht_add(HashTable *ht, uint32_t key, seq_t inc) {
    size_t idx = key & ht->mask;
    while (ht->used[idx]) {
        if (ht->keys[idx] == key) {
            ht->vals[idx] += inc;     /* merge counts for duplicate key */
            return;
        }
        idx = (idx + 1) & ht->mask;
    }
    ht->used[idx] = true;
    ht->keys[idx] = key;
    ht->vals[idx] = inc;
}

/* ---------------------------------------------------------------------- */
/*  Packing / unpacking of (x,y,z) into a 32‑bit key.                     */
/*  x <= 255 , y <= 255 , z <= 511   (holds for all needed n <= 200)    */

/* ---------------------------------------------------------------------- */
/*  Modular arithmetic helpers (used only for the BM algorithm).          */
/* ---------------------------------------------------------------------- */
static int mod_pow(int a, int e, int mod) {
    long long res = 1;
    long long base = a % mod;
    while (e) {
        if (e & 1) res = (res * base) % mod;
        base = (base * base) % mod;
        e >>= 1;
    }
    return (int)res;
}

static int mod_inv(int a, int mod) {
    return mod_pow(a, mod - 2, mod);      /* mod is prime here           */
}

/* ---------------------------------------------------------------------- */
/*  Berlekamp‑Massey algorithm, pure C.                                  */
/*  seq[0..n-1] are sequence values modulo mod.                          */
/*  Returns the recurrence coefficients (C[0..L-1]) such that          */
/*      seq[i] = SUM_{k=0}^{L-1} C[k] * seq[i-1-k]  (i>=L)            */
/*  The array is allocated with malloc and must be freed by the caller.  */
/* ---------------------------------------------------------------------- */
static int* bm_mod(const int *seq, size_t n, int mod, size_t *rec_len) {
    int *C = (int *)calloc(n + 1, sizeof(int));
    int *B = (int *)calloc(n + 1, sizeof(int));
    C[0] = 1;  B[0] = 1;
    int L = 0, m = 1;
    int b = 1;

    for (size_t i = 0; i < n; ++i) {
        long long d = 0;
        for (int j = 0; j <= L; ++j) {
            d = (d + (long long)C[j] * seq[i - j]) % mod;
        }
        if (d == 0) { m++; continue; }

        int coef = (int)(d * (long long)mod_inv(b, mod) % mod);
        int *T = (int *)malloc((m + L + 1) * sizeof(int));
        memcpy(T, C, (m + L + 1) * sizeof(int));

        if (C - T) {} /* silence unused warning */

        for (int j = 0; j <= L; ++j) {
            int val = C[m + j] - (int)((long long)coef * B[j] % mod);
            if (val < 0) val += mod;
            C[m + j] = val;
        }

        if (2 * L <= (int)i) {
            L = (int)(i + 1 - L);
            memcpy(B, T, (m + L + 1) * sizeof(int));
            b = (int)d;
            m = 1;
        } else {
            ++m;
        }
        free(T);
    }
    C[0] = 0;   /* discard leading 1 */
    *rec_len = L;
    int *rec = (int *)malloc(L * sizeof(int));
    for (int i = 0; i < L; ++i) {
        int v = C[i + 1];
        rec[i] = (v == 0) ? 0 : (mod - v) % mod;  /* positive coefficients */
    }
    free(C);
    free(B);
    return rec;
}

/* ---------------------------------------------------------------------- */
/*  Main function: a(n)                                                     */
/* ---------------------------------------------------------------------- */
seq_t a(idx_t n) {
    /* ──────────────────────  static initialization  ────────────────── */
    static bool initialized = false;
    static seq_t *cache = NULL;      /* memoised a(k) values */
    static size_t cache_cap = 0;
    static size_t cache_size = 0;

    /* recurrence coefficients */
    static size_t rec_len = 0;
    static seq_t *rec_coef = NULL;   /* rec_coef[0..rec_len-1] */

    if (!initialized) {
        /* --- compute first 200 terms exactly using DP -------------- */
        const size_t MAX_N = 200;
        seq_t *seq_val = (seq_t *)malloc((MAX_N + 1) * sizeof(seq_t));
        seq_val[0] = 1;

        /* hash tables for current and next step */
        HashTable cur, nxt;                      /* 2^21 slots ≈ 2 M entries */
        ht_init(&cur, 21);
        ht_init(&nxt, 21);

        ht_add(&cur, packuv(0, 0, 0), 1);        /* starting state */

        for (size_t step = 1; step <= MAX_N; ++step) {
            ht_clear(&nxt);                      /* reset the next table */

            for (size_t i = 0; i < cur.size; ++i) {
                if (!cur.used[i]) continue;
                uint32_t key = cur.keys[i];
                seq_t cnt   = cur.vals[i];

                int x, y, z;
                unpackuv(key, &x, &y, &z);

                /* transition 1: (-1,0,1)  requires x>0 */
                if (x > 0) {
                    uint32_t nk = packuv(x - 1, y, z + 1);
                    ht_add(&nxt, nk, cnt);
                }

                /* transition 2: (0,0,1)  always allowed */
                {
                    uint32_t nk = packuv(x, y, z + 1);
                    ht_add(&nxt, nk, cnt);
                }

                /* transition 3: (0,1,-1) requires z>0 */
                if (z > 0) {
                    uint32_t nk = packuv(x, y + 1, z - 1);
                    ht_add(&nxt, nk, cnt);
                }

                /* transition 4: (1,-1,0) requires y>0 */
                if (y > 0) {
                    uint32_t nk = packuv(x + 1, y - 1, z);
                    ht_add(&nxt, nk, cnt);
                }
            }

            /* sum of all counts in the new table = a[step] */
            seq_t sum = 0;
            for (size_t i = 0; i < nxt.size; ++i) {
                if (!nxt.used[i]) continue;
                sum += nxt.vals[i];
            }
            seq_val[step] = sum;

            /* prepare for next iteration */
            HashTable tmp = cur;
            cur = nxt;
            nxt = tmp;
        }

        /* --- find recurrence modulo two primes  ------------------- */
        const int primes[2] = { 1000000007, 1000000009 };
        int **mods = (int **)malloc(2 * sizeof(int *));
        size_t *recLens = (size_t *)malloc(2 * sizeof(size_t));

        for (int pi = 0; pi < 2; ++pi) {
            int *seq_mod = (int *)malloc((MAX_N + 1) * sizeof(int));
            for (size_t i = 0; i <= MAX_N; ++i) {
                seq_mod[i] = (int)(seq_val[i] % (seq_t)primes[pi]);
            }
            mods[pi] = bm_mod(seq_mod, MAX_N + 1, primes[pi], &recLens[pi]);
            free(seq_mod);
        }

        /* assume same order for both primes; take the larger one */
        rec_len = (recLens[0] > recLens[1]) ? recLens[0] : recLens[1];

        /* --- combine residues via CRT to get integer coefficients -- */
        rec_coef = (seq_t *)malloc(rec_len * sizeof(seq_t));
        for (size_t j = 0; j < rec_len; ++j) {
            seq_t val = 0;
            seq_t prod = 1;
            for (int pi = 0; pi < 2; ++pi) {
                int r = mods[pi][j];
                val += (seq_t)r * prod;
                prod *= (seq_t)primes[pi];
            }
            rec_coef[j] = val;
        }

        /* free BM structures */
        for (int pi = 0; pi < 2; ++pi) free(mods[pi]);
        free(mods);
        free(recLens);

        /* --- initialise memoisation cache with first rec_len terms --- */
        cache_size  = rec_len;
        cache_cap   = cache_size;
        cache       = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        for (size_t i = 0; i < rec_len; ++i) cache[i] = seq_val[i];

        free(seq_val);
        initialized = true;
    }

    /* --- answer the request ------------------------------------------- */
    if (n < cache_size) return cache[n];

    /* extend cache up to n */
    size_t target = (size_t)n;
    while (cache_cap <= target) {
        cache_cap = cache_cap ? cache_cap * 2 : 1;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    }

    for (size_t i = cache_size; i <= target; ++i) {
        seq_t val = 0;
        for (size_t j = 0; j < rec_len; ++j) {
            val += rec_coef[j] * cache[i - 1 - j];
        }
        cache[i] = val;
    }
    cache_size = target + 1;
    return cache[target];
}
