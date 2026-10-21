/*
 * A211781  Numbers n such that Sum_(d_<n | n) tau(d_<n) / (d_<n) is an
 *          integer, where d_<n = divisors of n that are less than n, tau(x) =
 *          A000005(x).
 * https://oeis.org/A211781
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 50, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A211781 – Numbers n > 1 such that  
 *  ( sum_{d|n, d<n} τ(d) / d ) is an integer.
 *  The code tests successive integers and keeps the ones that satisfy the
 *  property.  It memoises previous results so that out‑of‑order calls are
 *  handled efficiently. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/*----- cache for divisor counts ---------------------------------------*/
static uint64_t *tau_cache = NULL;   /* tau_cache[i] = τ(i), 0 = not yet computed */
static size_t tau_cap = 0;

/*  initialise the cache for the smallest values                       */
static void init_tau_cache(void)
{
    if (tau_cap == 0) {
        tau_cap = 16;
        tau_cache = (uint64_t*)malloc(tau_cap * sizeof(uint64_t));
        for (size_t i = 0; i < tau_cap; ++i) tau_cache[i] = 0;
        tau_cache[1] = 1;          /* τ(1) = 1 */
    }
}

/*  compute divisor exponent product ( τ(n) )  */
static uint64_t tau_compute(uint64_t n)
{
    if (n == 0) return 0;

    /* enlarge cache if needed */
    if (n >= tau_cap) {
        size_t new_cap = tau_cap * 2;
        if (new_cap <= n) new_cap = n + 1;
        uint64_t *new_cache = (uint64_t*)realloc(tau_cache, new_cap * sizeof(uint64_t));
        for (size_t i = tau_cap; i < new_cap; ++i) new_cache[i] = 0;
        tau_cache = new_cache;
        tau_cap = new_cap;
    }

    if (tau_cache[n] != 0) return tau_cache[n];

    uint64_t result = 1;
    uint64_t m = n;
    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            uint64_t cnt = 0;
            while (m % p == 0) {
                m /= p;
                ++cnt;
            }
            result *= (cnt + 1);
        }
    }
    if (m > 1) result *= 2;
    tau_cache[n] = result;
    return result;
}

/*  test whether a candidate satisfies the defining property  */
static bool qualifies(uint64_t m)
{
    if (m <= 1) return false;

    seq_t T = 0;                     /* numerator sum */
    for (uint64_t d = 1; d * d <= m; ++d) {
        if (m % d == 0) {
            uint64_t d1 = d, d2 = m / d;

            if (d1 < m) {
                seq_t term = ((seq_t)tau_compute(d1)) * ((seq_t)(m / d1));
                T += term;
            }
            if (d2 != d1 && d2 < m) {
                seq_t term = ((seq_t)tau_compute(d2)) * ((seq_t)(m / d2));
                T += term;
            }
        }
    }
    return ((T % (seq_t)m) == 0);
}

/*----- dynamic array for the sequence ---------------------------------*/
static seq_t *seq = NULL;   /* stored terms, indexed from 0 */
static size_t seq_len = 0;  /* number of terms stored */
static size_t seq_cap = 0;  /* current allocated size */

/*  enlarge the array if needed  */
static void ensure_seq_cap(size_t min_needed)
{
    if (seq_cap < min_needed) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 16;
        if (new_cap < min_needed) new_cap = min_needed;
        seq = (seq_t*)realloc(seq, new_cap * sizeof(seq_t));
        seq_cap = new_cap;
    }
}

/*----- global state for incremental generation -----------------------*/
static uint64_t last_checked = 1;  /* last integer examined */

/*=====================================================================*/
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined, but return 0 */

    init_tau_cache();     /* make sure τ cache is ready */

    /* generate terms until we have at least n of them */
    while (seq_len < (size_t)n) {
        ++last_checked;
        if (qualifies(last_checked)) {
            ensure_seq_cap(seq_len + 1);
            seq[seq_len] = (seq_t)last_checked;
            ++seq_len;
        }
    }
    return seq[n - 1];
}
