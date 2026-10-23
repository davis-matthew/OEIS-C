/*
 * A174235  The absolute values of (n-th number k such that k*4-+1 is twin
 *          prime pair minus n-th number m such that m*4+2-+1 is twin prime
 *          pair).
 * https://oeis.org/A174235
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Abs(A045753(n)-A123986(n))
 * Range:     u8 n <= 17, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A045753(n) is the n-th integer k such that 4k-1 and 4k+1 are both prime. */
/* A123986(n) is the n-th integer m such that 4m+1 and 4m+3 are both prime. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ----- 64‑bit Miller–Rabin primality test ----- */
static inline uint64_t mulmod_uint64(uint64_t a, uint64_t b, uint64_t mod)
{
    return (unsigned __int128)a * b % mod;
}

static inline uint64_t modpow_uint64(uint64_t a, uint64_t d, uint64_t mod)
{
    uint64_t r = 1;
    while (d) {
        if (d & 1) r = mulmod_uint64(r, a, mod);
        a = mulmod_uint64(a, a, mod);
        d >>= 1;
    }
    return r;
}

static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    static const uint64_t small_primes[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    if (n < 40) {
        for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i)
            if (n == small_primes[i]) return true;
        return false;
    }
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i)
        if (n % small_primes[i] == 0) return n == small_primes[i];

    uint64_t d = n - 1, s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }
    static const uint64_t bases[] = {2,3,5,7,11,13};
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i];
        if (a >= n) continue;
        uint64_t x = modpow_uint64(a, d, n);
        if (x == 1 || x == n-1) continue;
        bool composite = true;
        for (uint64_t r = 1; r < s; ++r) {
            x = mulmod_uint64(x, x, n);
            if (x == n-1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

/* ----- Utility to grow dynamic arrays of seq_t ----- */
static void ensure_capacity_seq_t(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t newcap = (*cap) ? (*cap << 1) : 64;
        while (newcap < need) newcap <<= 1;
        seq_t *newarr = (seq_t *)realloc(*arr, newcap * sizeof(seq_t));
        if (!newarr) { /* allocation failure */
            /* In an OEIS context this never happens for the requested sizes */
            exit(1);
        }
        *arr = newarr;
        *cap = newcap;
    }
}

/* ----- Main sequence function ----- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* out of offset – undefined behavior not needed */

    /* Static state for the two twin‑prime sequences */
    static seq_t *seqA045753 = NULL;
    static size_t lenA045753 = 0;
    static size_t capA045753 = 0;
    static uint64_t curk = 1;           /* next k to test */

    static seq_t *seqA123986 = NULL;
    static size_t lenA123986 = 0;
    static size_t capA123986 = 0;
    static uint64_t curm = 1;           /* next m to test */

    /* Grow first sequence up to n terms */
    while (lenA045753 < (size_t)n) {
        uint64_t p1 = 4 * curk - 1;
        uint64_t p2 = 4 * curk + 1;
        if (is_prime_uint64(p1) && is_prime_uint64(p2)) {
            ensure_capacity_seq_t(&seqA045753, &capA045753, lenA045753 + 1);
            seqA045753[lenA045753++] = (seq_t)curk;
        }
        ++curk;
    }

    /* Grow second sequence up to n terms */
    while (lenA123986 < (size_t)n) {
        uint64_t p1 = 4 * curm + 1;
        uint64_t p2 = 4 * curm + 3;
        if (is_prime_uint64(p1) && is_prime_uint64(p2)) {
            ensure_capacity_seq_t(&seqA123986, &capA123986, lenA123986 + 1);
            seqA123986[lenA123986++] = (seq_t)curm;
        }
        ++curm;
    }

    seq_t v1 = seqA045753[(size_t)(n - 1)];
    seq_t v2 = seqA123986[(size_t)(n - 1)];
    return (v1 >= v2) ? (v1 - v2) : (v2 - v1);
}
