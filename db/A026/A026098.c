/*
 * A026098  Triangular array T read by rows: T(1,1)=1, T(2,1)=3, T(2,2)=2; for
 *          n >= 3, T(n,1)=prime(n) and for k=2,3,...,n, T(n,k) =
 *          m*prime(n+1-k), where m is the least positive integer such that
 *          m*p(n+1-k) is not any T(i,j) for 1<=i<=n-1 nor any T(n,j) for
 *          j<=k-1.
 * https://oeis.org/A026098
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   A026098 (triangular array permutation)
 * Range:     u8 n <= 157, u16 n <= 5050, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implemented by constructing the triangle on demand and memoising
   all entries. For each required value we extend the triangle
   until the requested index is available. */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* ---------- Prime generator (trial division, lazy) ------------------ */
static uint32_t *prime_cache = NULL;   /* 1‑indexed array of primes */
static size_t   prime_cap   = 0;
static size_t   prime_cnt   = 0;
static uint32_t next_candidate = 3;     /* next odd to try */

/* Ensure at least k primes are available (k>=1). */
static void ensure_prime_n(idx_t k)
{
    if (prime_cnt >= (size_t)k) return;

    /* initial allocation */
    if (prime_cache == NULL) {
        prime_cap = 128;
        prime_cache = (uint32_t*)malloc(prime_cap * sizeof(uint32_t));
        if (!prime_cache) abort();
    }
    /* grow if necessary */
    if (prime_cap < (size_t)k) {
        size_t new_cap = prime_cap ? prime_cap << 1 : 128;
        while (new_cap < (size_t)k) new_cap <<= 1;
        uint32_t *tmp = (uint32_t*)realloc(prime_cache, new_cap * sizeof(uint32_t));
        if (!tmp) abort();
        prime_cache = tmp;
        prime_cap   = new_cap;
    }

    /* start with 2 if empty */
    if (prime_cnt == 0) {
        prime_cache[0] = 2;
        prime_cnt = 1;
        next_candidate = 3;
    }

    /* generate further primes */
    while (prime_cnt < (size_t)k) {
        uint32_t p = next_candidate;
        int is_p = 1;
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint64_t sq = (uint64_t)prime_cache[i] * prime_cache[i];
            if (sq > p) break;
            if (p % prime_cache[i] == 0) { is_p = 0; break; }
        }
        if (is_p) prime_cache[prime_cnt++] = p;
        next_candidate += 2;               /* skip evens */
    }
}

/* Return the k‑th prime (1‑indexed). */
static uint32_t get_prime(idx_t k)
{
    if (k == 0) return 0;
    ensure_prime_n(k);
    return prime_cache[k - 1];
}

/* ---------- Sequence (triangle) storage ---------------------------- */
static seq_t *seq_cache = NULL;      /* a(1) .. a(seq_size)           */
static size_t seq_cap    = 0;
static size_t seq_size   = 0;         /* number of terms stored */
static idx_t  next_row   = 0;         /* rows processed so far (0=none) */

/* Bitset for used values (they form a permutation of N). */
static uint8_t *used_bits     = NULL; /* 0/1 bits */
static size_t  used_cap_bytes = 0;
static idx_t   used_max_val   = 0;    /* largest value seen */

/* Ensure seq_cache holds at least need slots. */
static void ensure_seq_cap(size_t need)
{
    if (need <= seq_cap) return;

    size_t new_cap = seq_cap ? seq_cap << 1 : 128;
    while (new_cap < need) new_cap <<= 1;

    seq_t *tmp = (seq_t*)realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    seq_cache = tmp;
    seq_cap   = new_cap;
}

/* Ensure used_bits covers value val. */
static void ensure_used_cap(idx_t val)
{
    if (val <= used_max_val) return;
    used_max_val = val;
    size_t need_bytes = (size_t)(val >> 3) + 1;
    if (need_bytes <= used_cap_bytes) return;
    size_t new_cap = used_cap_bytes ? used_cap_bytes << 1 : 64;
    while (new_cap < need_bytes) new_cap <<= 1;

    uint8_t *tmp = (uint8_t*)realloc(used_bits, new_cap);
    if (!tmp) abort();

    /* zero newly allocated tail */
    memset(tmp + used_cap_bytes, 0, new_cap - used_cap_bytes);

    used_bits     = tmp;
    used_cap_bytes = new_cap;
}

/* Query if value v has already appeared. */
static int is_used(idx_t v)
{
    return used_bits[v >> 3] & (1U << (v & 7U));
}

/* Mark value v as used. */
static void mark_used(idx_t v)
{
    used_bits[v >> 3] |= 1U << (v & 7U);
}

/* Generate rows until we have at least n terms. */
static void ensure_n_terms(idx_t n)
{
    if (seq_size >= (size_t)n) return;

    /* initial allocation if necessary */
    if (seq_cache == NULL) {
        seq_cap    = 128;
        seq_cache  = (seq_t*)malloc(seq_cap * sizeof(seq_t));
        if (!seq_cache) abort();
    }
    if (used_bits == NULL) {
        used_cap_bytes = 64;
        used_bits = (uint8_t*)malloc(used_cap_bytes);
        if (!used_bits) abort();
        memset(used_bits, 0, used_cap_bytes);
    }

    if (next_row == 0) {              /* first row not yet processed */
        seq_cache[0] = (seq_t)1;
        seq_size    = 1;
        next_row    = 1;
        ensure_used_cap(1);
        mark_used(1);
    }

    /* generate rows until we have enough terms */
    while (seq_size < (size_t)n) {
        idx_t row = next_row + 1;     /* rows are 1‑indexed */

        /* T(row,1) = prime(row) */
        seq_t val = (seq_t)get_prime(row);
        ensure_seq_cap(seq_size + 1);
        seq_cache[seq_size++] = val;
        ensure_used_cap((idx_t)val);
        mark_used((idx_t)val);

        /* fill k = 2 .. row */
        for (idx_t k = 2; k <= row; ++k) {
            idx_t p_idx = row + 1 - k;             /* prime index */
            seq_t p = (seq_t)get_prime(p_idx);

            /* find smallest m such that m*p unused */
            idx_t m = 1;
            seq_t cand;
            while (1) {
                cand = (seq_t)m * p;
                ensure_used_cap((idx_t)cand);
                if (!is_used((idx_t)cand)) break;
                ++m;
            }

            /* store value */
            ensure_seq_cap(seq_size + 1);
            seq_cache[seq_size++] = cand;
            mark_used((idx_t)cand);
        }

        ++next_row;
    }
}

/* -------------------------------------------------------------- */
/* External function required by the framework */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                /* undefined for n < 1 */
    ensure_n_terms(n);
    return seq_cache[n - 1];
}
