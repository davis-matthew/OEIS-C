/*
 * A084689  3^n uses only distinct decimal digits.
 * https://oeis.org/A084689
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, nonn, full, base
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A084689: a(n) = the n‑th exponent k such that 3^k uses only distinct decimal digits.
   The sequence is finite (length 9).  The function finds these exponents by
   brute‑force search and caches the results. */

#include <stdbool.h>
#include <stddef.h>

/* Test if a non‑negative seq_t has all distinct decimal digits. */
static bool distinct_digits(seq_t x)
{
    unsigned mask = 0;
    if (x == 0)                /* digit '0' occurs only once */
        return true;

    while (x > 0) {
        /* Extract the least significant decimal digit.  The modulus
           returns a seq_t; it can be cast to an unsigned long long
           because the value is 0–9. */
        seq_t dseq = x % 10;
        unsigned long long d = (unsigned long long)dseq;
        if (mask & (1U << d))
            return false;       /* repeated digit found */
        mask |= 1U << d;
        x /= 10;
    }
    return true;
}

seq_t a(idx_t n)
{
    static seq_t cache[9];
    static unsigned int count = 0;
    static bool initialized = false;

    if (!initialized) {
        seq_t pow = 1;          /* 3^0 */
        for (seq_t k = 0; count < 9; ++k) {
            if (distinct_digits(pow))
                cache[count++] = k;  /* store the exponent */
            pow *= 3;            /* next power of 3 */
        }
        initialized = true;
    }

    if (n <= 0 || n > count)      /* undefined for n>9 */
        return (seq_t)0;
    return cache[n - 1];
}
