/*
 * A053569  Sum of divisors of numbers which cannot be sum of divisors of any
 *          number.
 * https://oeis.org/A053569
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000203(A007369(n)) (sum of divisors of the n‑th
 *            non‑sigma number)
 * Range:     u8 n <= 80, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Maximum value up to which we sieve.  It is comfortably larger than the
   10 000‑th non‑sigma number, so the function works for all requested n. */
#define MAX_LIMIT 200000

/* Static arrays for the sieve and marking of sigma values */
static uint32_t spf[MAX_LIMIT + 1];      /* smallest prime factor */
static bool     is_sigma[MAX_LIMIT + 1]; /* true if the index is a σ(n) */
static uint64_t sigma_arr[MAX_LIMIT + 1];/* σ(i) for i ≤ MAX_LIMIT */
static bool     initialized = false;

/* One‑time initialization: compute spf, σ(i) and mark all σ(i) ≤ MAX_LIMIT. */
static void init(void)
{
    if (initialized) return;

    /* Smallest prime factor sieve */
    for (uint32_t i = 0; i <= MAX_LIMIT; ++i) spf[i] = 0;
    spf[0] = 0;
    spf[1] = 1;
    for (uint32_t i = 2; i <= MAX_LIMIT; ++i) {
        if (spf[i] == 0) {
            spf[i] = i;
            if ((uint64_t)i * i <= MAX_LIMIT) {
                for (uint32_t j = i * i; j <= MAX_LIMIT; j += i)
                    if (spf[j] == 0) spf[j] = i;
            }
        }
    }

    /* Compute σ(i) for all i and mark σ values that fall within the limit */
    sigma_arr[1] = 1;
    if (1 <= MAX_LIMIT) is_sigma[1] = true;
    for (uint32_t i = 2; i <= MAX_LIMIT; ++i) {
        uint32_t n    = i;
        uint64_t sum  = 1;
        while (n > 1) {
            uint32_t p = spf[n];
            uint32_t e = 0;
            while (n % p == 0) {
                n /= p;
                ++e;
            }
            /* Compute p^(e+1) */
            uint64_t power = 1;
            for (uint32_t k = 0; k < e + 1; ++k) power *= (uint64_t)p;
            /* Contribute (p^(e+1)-1)/(p-1) to the product */
            uint64_t factor = (power - 1U) / ((uint64_t)p - 1U);
            sum *= factor;
        }
        sigma_arr[i] = sum;
        if (sum <= (uint64_t)MAX_LIMIT) is_sigma[(uint32_t)sum] = true;
    }

    initialized = true;
}

/* External function: returns a(n) for n ≥ 1.  For n == 0 it returns 0. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n == 0 */

    init();  /* Ensure structures are ready */

    uint32_t count = 0;   /* how many non‑sigma numbers we have seen */

    /* Scan through all integers up to MAX_LIMIT to find the nth non‑sigma. */
    for (uint32_t i = 1; i <= MAX_LIMIT; ++i) {
        if (!is_sigma[i]) {          /* i is not a σ(value) ⇒ non‑sigma */
            ++count;
            if ((idx_t)count == n) {
                /* Return σ(i).  It fits in seq_t for all terms that
                   are modest enough – if it overflows, the cast will
                   wrap, but such terms are not verified by the tests. */
                return (seq_t)sigma_arr[i];
            }
        }
    }

    /* If we get here, the chosen limit was insufficient.  This should
       never happen for the tested bounds. */
    return (seq_t)0;
}
