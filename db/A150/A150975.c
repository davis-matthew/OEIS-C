/*
 * A150975  Number of walks within N^3 (the first octant of Z^3) starting at
 *          (0,0,0) and consisting of n steps taken from {(-1, -1, 0), (-1, 0,
 *          0), (1, 0, 1), (1, 1, -1), (1, 1, 1)}.
 * https://oeis.org/A150975
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, walk
 * Formula:   Number of walks within the first octant of ℤ³ starting at
 *            (0,0,0) with step set {(-1,-1,0), (-1,0,0), (1,0,1), (1,1,-1),
 *            (1,1,1)}
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Includes */
#include <stdint.h>
#include <stdlib.h>

/* ---------- Utility functions for modular arithmetic ---------- */
static uint64_t modpow_uint64(uint64_t a, uint64_t e, uint64_t mod) {
    uint64_t r = 1 % mod;
    while (e) {
        if (e & 1) r = (unsigned __int128)r * a % mod;
        a = (unsigned __int128)a * a % mod;
        e >>= 1;
    }
    return r;
}

/* ---------- Berlekamp–Massey over a prime field ---------------- */
static size_t berlekamp_massey(const uint64_t *s, size_t n, uint64_t mod,
                               int64_t **out_coeff, size_t *out_len) {
    size_t d = 0;                       /* current order */
    uint64_t C[256] = {0}, B[256] = {0};
    C[0] = B[0] = 1;
    uint64_t b = 1;
    size_t m = 1;

    for (size_t i = 0; i < n; ++i) {
        uint64_t disc = 0;
        for (size_t j = 0; j <= d; ++j)
            disc = (disc + C[j] * s[i - j]) % mod;

        if (disc == 0) {
            m++;
            continue;
        }

        uint64_t coef = disc * modpow_uint64(b, mod - 2, mod) % mod;
        int64_t T[256];
        for (size_t j = 0; j < 256; ++j) T[j] = (int64_t)C[j];

        if (d + 1 < (size_t)m + 256) {
            for (size_t j = d + 1; j < m + 256; ++j) C[j] = 0;
        }

        for (size_t j = 0; j < 256; ++j) {
            uint64_t val = C[j + m];
            val -= coef * B[j] % mod;
            if ((int64_t)val < 0) val += mod;
            C[j + m] = val;
        }

        if (2 * d <= i) {
            d = i + 1 - d;
            for (size_t j = 0; j < 256; ++j) B[j] = T[j];
            b = disc;
            m = 1;
        } else {
            m++;
        }
    }

    *out_len = d;
    *out_coeff = (int64_t *)malloc(sizeof(int64_t) * d);
    for (size_t i = 0; i < d; ++i) {
        uint64_t val = C[i + 1] % mod;
        uint64_t coeff = (mod - val) % mod;
        if (coeff > mod / 2) coeff = (int64_t)coeff - (int64_t)mod;
        (*out_coeff)[i] = (int64_t)coeff;
    }
    return d;
}

/* ---------- Small DP to obtain initial terms ----------------- */
#define DP_MAX 120 /* enough to get initial data for recurrence detection */
static void compute_small_terms(uint64_t *buf, size_t len) {
    size_t dim = DP_MAX + 1;
    size_t cells = dim * dim * dim;
    uint64_t *cur = (uint64_t *)calloc(cells, sizeof(uint64_t));
    uint64_t *nxt = (uint64_t *)calloc(cells, sizeof(uint64_t));
    if (!cur || !nxt) exit(1);

    /* start position (0,0,0) */
    cur[0] = 1;

    for (size_t step = 0; step <= DP_MAX; ++step) {
        /* sum of current states */
        uint64_t total = 0;
        for (size_t idx = 0; idx < cells; ++idx) total += cur[idx];
        if (step < len) buf[step] = total;

        if (step == DP_MAX) break;

        /* propagate */
        for (size_t x = 0; x <= DP_MAX; ++x) {
            for (size_t y = 0; y <= DP_MAX; ++y) {
                for (size_t z = 0; z <= DP_MAX; ++z) {
                    size_t idx = x * dim * dim + y * dim + z;
                    uint64_t val = cur[idx];
                    if (!val) continue;

                    /* step (-1,-1,0) */
                    if (x >= 1 && y >= 1) {
                        size_t nidx = (x - 1) * dim * dim + (y - 1) * dim + z;
                        nxt[nidx] += val;
                    }
                    /* step (-1,0,0) */
                    if (x >= 1) {
                        size_t nidx = (x - 1) * dim * dim + y * dim + z;
                        nxt[nidx] += val;
                    }
                    /* step (1,0,1) */
                    if (x + 1 <= DP_MAX && z + 1 <= DP_MAX) {
                        size_t nidx = (x + 1) * dim * dim + y * dim + (z + 1);
                        nxt[nidx] += val;
                    }
                    /* step (1,1,-1) */
                    if (x + 1 <= DP_MAX && y + 1 <= DP_MAX && z >= 1) {
                        size_t nidx = (x + 1) * dim * dim + (y + 1) * dim + (z - 1);
                        nxt[nidx] += val;
                    }
                    /* step (1,1,1) */
                    if (x + 1 <= DP_MAX && y + 1 <= DP_MAX && z + 1 <= DP_MAX) {
                        size_t nidx = (x + 1) * dim * dim + (y + 1) * dim + (z + 1);
                        nxt[nidx] += val;
                    }
                }
            }
        }
        /* swap */
        uint64_t *tmp = cur;
        cur = nxt;
        nxt = tmp;
        /* clear next */
        for (size_t i = 0; i < cells; ++i) nxt[i] = 0;
    }
    free(cur);
    free(nxt);
}

/* ---------- Main sequence function ------------------------------ */
seq_t a(idx_t n) {
    /* static data */
    static int initialized = 0;
    static size_t rec_len = 0;
    static int64_t *rec_coeff = NULL;       /* recurrence coefficients (signed) */
    static seq_t *init_vals = NULL;         /* first rec_len terms */
    static seq_t *cache = NULL;             /* computed values cache */
    static size_t cache_size = 0;

    /* initial construction */
    if (!initialized) {
        /* compute a small list of terms */
        size_t tiny_len = DP_MAX + 1;
        uint64_t *tiny = (uint64_t *)malloc(tiny_len * sizeof(uint64_t));
        compute_small_terms(tiny, tiny_len);

        /* find minimal linear recurrence using BM over a prime */
        const uint64_t PRIME = 5000003ULL;  /* > 2 * expected coeff magnitude */
        size_t rec_order = 0;
        int64_t *coeff = NULL;
        berlekamp_massey(tiny, tiny_len, PRIME, &coeff, &rec_order);

        /* store recurrence */
        rec_len = rec_order;
        rec_coeff = coeff;

        /* store initial terms */
        init_vals = (seq_t *)malloc(rec_len * sizeof(seq_t));
        for (size_t i = 0; i < rec_len; ++i)
            init_vals[i] = (seq_t)tiny[i];

        /* initialize cache */
        cache_size = rec_len;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        for (size_t i = 0; i < rec_len; ++i)
            cache[i] = init_vals[i];

        free(tiny);
        initialized = 1;
    }

    /* ensure cache large enough */
    if ((size_t)n >= cache_size) {
        size_t new_size = (size_t)n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache) exit(1);
        cache = new_cache;

        for (size_t i = cache_size; i <= n; ++i) {
            seq_t sum = 0;
            for (size_t j = 0; j < rec_len; ++j) {
                seq_t prod = cache[i - 1 - j] * rec_coeff[j];
                sum += prod;
            }
            cache[i] = sum;
        }
        cache_size = new_size;
    }

    return cache[(size_t)n];
}
