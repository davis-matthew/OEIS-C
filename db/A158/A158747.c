/*
 * A158747  Triangle read by rows: T(n,m)=prime( 1+prime(n+1)-prime(m+1) ).
 * https://oeis.org/A158747
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A158747: Triangle read by rows.
 *
 * Formula: T(n,m) = prime(1 + prime(n+1) - prime(m+1))
 *           with 0 ≤ n, 0 ≤ m ≤ n.
 * The function a(idx_t n) returns the n-th term in the row‑by‑row
 * enumeration of this triangle, i.e. a(0)=2, a(1)=3, a(2)=2, …
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

#if defined(__cplusplus)
#define OMALLOC(t, sz) reinterpret_cast<t*>(malloc(sz))
#define OREALLOC(t, ptr, sz) reinterpret_cast<t*>(realloc(ptr, sz))
#else
#define OMALLOC(t, sz) ((t*)malloc(sz))
#define OREALLOC(t, ptr, sz) ((t*)realloc(ptr, sz))
#endif

/* ---------- static prime cache (1‑based index) ---------- */

static seq_t *prime_cache = NULL;        /* array of primes */
static idx_t prime_cnt  = 0;             /* number of primes stored */
static idx_t prime_lim  = 0;             /* allocated size of array */
static seq_t next_candidate = 2;         /* next integer to test for primality */

/* enlarge cache and add primes until it contains at least 'need' primes */
static void ensure_primes(idx_t need)
{
    if (!prime_cache) {
        prime_lim = 16;
        prime_cache = OMALLOC(seq_t, prime_lim * sizeof(seq_t));
        prime_cnt = 0;
        next_candidate = 2;
    }

    while (prime_cnt < need) {
        seq_t cand = next_candidate;
        ++next_candidate;

        /* test cand for primality using previously found primes */
        int is_prime = 1;
        for (idx_t i = 0; i < prime_cnt &&
             (uint64_t)prime_cache[i] * (uint64_t)prime_cache[i] <= (uint64_t)cand; ++i) {
            if ((uint64_t)cand % (uint64_t)prime_cache[i] == 0) {
                is_prime = 0;
                break;
            }
        }

        if (is_prime) {
            if (prime_cnt >= prime_lim) {
                prime_lim *= 2;
                prime_cache = OREALLOC(seq_t, prime_cache, prime_lim * sizeof(seq_t));
            }
            prime_cache[prime_cnt++] = cand;
        }
    }
}

/* return the k-th prime (k ≥ 1) */
static seq_t get_prime_idx(idx_t k)
{
    if (k == 0) return 0;          /* undefined, but keep safe */
    ensure_primes(k);
    return prime_cache[k - 1];
}

/* ---------- locate the row containing index n (0‑based) ---------- */
static idx_t locate_row(idx_t n)
{
    idx_t lo = 0, hi = 1;
    /* increase hi until hi*(hi+1)/2 > n */
    while ((unsigned __int128)hi * (hi + 1) / 2 <= n) hi <<= 1;

    while (lo < hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        unsigned __int128 tri = (unsigned __int128)mid * (mid + 1) / 2;
        if (tri <= n) lo = mid + 1;
        else          hi = mid;
    }
    return lo - 1;   /* row index */
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    /* find row r and column c (0‑based) in the triangle */
    idx_t r = locate_row(n);
    idx_t accum = (unsigned __int128)r * (r + 1) / 2;
    idx_t c = n - accum;

    /* compute primes needed for the formula */
    seq_t p_r = get_prime_idx(r + 1);
    seq_t p_c = get_prime_idx(c + 1);

    /* difference in the formula, always positive */
    idx_t diff_idx = 1 + (idx_t)(p_r - p_c);

    /* final value: the diff_idx‑th prime */
    return get_prime_idx(diff_idx);
}
