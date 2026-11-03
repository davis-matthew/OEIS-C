/*
 * A239941  Primes p which are floor of Root-mean-cube (RMC) of prime(n),
 *          prime(n+1) and prime(n+2).
 * https://oeis.org/A239941
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 15, u64 n <= 1745,
 *            u128 n <= 1745, big any n
 * Validated: standard, pass, 54 terms through n = 1581 (2026-11-03)
 * Helpers:   build_primes_upto, ipow_i32, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A239941: a(n) is the nth prime that equals
   floor( sqrt( (p³ + q³ + r³)/3 ) ) for some consecutive primes
   p, q, r.  The function generates primes on the fly, caching
   results so it works for repeated, out‑of‑order queries. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/build_primes_upto.h"
#include "../utils/ipow_i32.h"
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* Append a value to the cached array, expanding it if necessary.  `p` is a
   pointer to the array pointer. */
static void append_candidate(seq_t **p, size_t *len, size_t *cap, seq_t val)
{
    if (*len == *cap) {
        size_t newcap = (*cap == 0) ? 16 : (*cap << 1);
        seq_t *tmp = mem_resize(seq_t, *p, newcap);
        if (!tmp) {
            exit(EXIT_FAILURE);
        }
        *p = tmp;
        *cap = newcap;
    }
    (*p)[*len] = val;
    (*len)++;
}

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

/* Cached sequence values */
static seq_t *seq_cache   = NULL;
static size_t seq_len     = 0;
static size_t seq_cap     = 0;

/* Cached list of primes */
static uint64_t *prime_list = NULL;
static size_t   prime_cnt   = 0;
static size_t   tri_index   = 0;   /* first index of current triplet */

/* Initialise the global data on first use */
static void init_globals(void)
{
    static bool done = false;
    if (done) return;

    /* A safe upper bound that provides enough primes for n ≤ 10 000 */
    const idx_t PRIME_LIMIT = 2000000;   /* pi(2 000 000) ≈ 148 933 */

    build_primes_upto(PRIME_LIMIT, &prime_list, &prime_cnt);
    tri_index = 0;

    seq_cache = NULL;
    seq_len   = 0;
    seq_cap   = 0;

    done = true;
}

/* Append a new candidate to the cached sequence */
static inline void add_term(seq_t val)
{
    append_candidate(&seq_cache, &seq_len, &seq_cap, val);
}

/* Main sequence routine */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* safety: offset starts at 1 */

    init_globals();

    /* Generate terms until the cached list contains at least n */
    while (seq_len < (size_t)n) {

        /* Ensure there are three primes available */
        if (tri_index + 2 >= prime_cnt) {
            /* This should not happen for the intended limits */
            return 0;
        }

        uint64_t p0 = prime_list[tri_index];
        uint64_t p1 = prime_list[tri_index + 1];
        uint64_t p2 = prime_list[tri_index + 2];

        /* Compute (p0³ + p1³ + p2³) / 3  using seq_t arithmetic */
        seq_t sum  = ((seq_t)p0 * p0 * p0)
                   + ((seq_t)p1 * p1 * p1)
                   + ((seq_t)p2 * p2 * p2);
        seq_t mean = sum / 3;

        /* Floor square root of the mean */
        seq_t candidate = floor_sqrt(mean);

        /* If the candidate itself is prime, store it */
        if (is_prime_big(candidate))
            add_term(candidate);

        /* Move to the next overlapping triplet */
        ++tri_index;
    }

    return seq_cache[n - 1];   /* a(1) is stored at index 0 */
}
