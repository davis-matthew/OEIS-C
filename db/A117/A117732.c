/*
 * A117732  Numbers k that divide s(k)^w(k), where s(k) is the sum of the
 *          digits of k and w(k) is the digital root of k.
 * https://oeis.org/A117732
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A117732: numbers k that divide s(k)^w(k) */
/* We generate terms on demand by testing successive integers k for the
 * condition k | s(k)^w(k), where s(k) is the digit sum and
 * w(k) is the digital root (1+((k-1)%9)).  A simple sieve-like
 * on-the-fly search is used. */

#include <stdint.h>
#include <stdlib.h>

/* maximum number of terms we can store (enough for n <= 10000) */
#define MAX_TERMS 200000

static seq_t terms[MAX_TERMS];
static size_t term_count = 0;
static seq_t last_checked_k = 1;   /* k to start the next search */

/* Compute (base^exp) mod mod, exp is small (<=9) */
static inline seq_t modpow(seq_t base, int exp, seq_t mod)
{
    if (mod == 0) return 0; /* avoid division by zero, though k>=1 */
    seq_t result = 1 % mod;
    base %= mod;
    while (exp > 0) {
        if (exp & 1)
            result = (result * base) % mod;
        exp >>= 1;
        if (exp)
            base = (base * base) % mod;
    }
    return result;
}

/* Complementary: compute the digit sum of x */
static inline seq_t digit_sum_local(seq_t x)
{
    seq_t sum = 0;
    while (x > 0) {
        sum += x % 10;
        x /= 10;
    }
    return sum;
}

seq_t a(idx_t n)
{
    /* Return cached term if already computed */
    if (n <= term_count)
        return terms[n - 1];

    /* Start searching from the last checked integer */
    seq_t k;
    for (k = last_checked_k; term_count < n; ++k) {
        /* digit sum s(k) */
        seq_t s = digit_sum_local(k);

        /* digital root w(k) = 1 + ((k-1) % 9) */
        seq_t rem = (k - (seq_t)1) % (seq_t)9;
        int w   = (int)(rem + (seq_t)1);

        /* check if k divides s^w */
        if (modpow(s, w, k) == (seq_t)0) {
            if (term_count < MAX_TERMS)
                terms[term_count++] = k;
        }
    }

    /* Update where we left off for future calls */
    last_checked_k = k;

    return terms[n - 1];
}
