/*
 * A116081  Final nonzero digit of n^n.
 * https://oeis.org/A116081
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, base, nonn, cons
 * Formula:   a(n) = last non‑zero digit of n^n
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* n >= 1 for the official sequence offset */
    if (n == 0)
        return 0;

    /* Count powers of 2 and 5 in n */
    idx_t a = 0, b = 0;
    idx_t tmp = n;
    while ((tmp & 1ULL) == 0) { a++; tmp >>= 1; }
    tmp = n;
    while (tmp % 5ULL == 0) { b++; tmp /= 5; }

    /* Remaining factor after cancelling trailing zeros */
    seq_t mult = 1;
    if (a > b) {
        /* exponent of 2 that remains: (a‑b) * n  */
        idx_t e_mod = ((a - b) % 4) * (n % 4) % 4;
        /* 2^k mod 10 where k>0 and k%4==0 gives 6 */
        if (e_mod == 0)
            mult = 6;
        else if (e_mod == 1)
            mult = 2;
        else if (e_mod == 2)
            mult = 4;
        else /* e_mod == 3 */
            mult = 8;
    } else if (b > a) {
        /* any positive power of 5 gives 5 mod 10 */
        mult = 5;
    }

    /* Base m = n / (2^a * 5^b)  (coprime with 10) */
    idx_t tmp2 = n;
    while (tmp2 % 2ULL == 0) tmp2 /= 2;
    while (tmp2 % 5ULL == 0) tmp2 /= 5;
    seq_t base = (seq_t)(tmp2 % 10);   /* base ∈ {1,3,7,9} */

    /* Compute base^n mod 10.  Since base is coprime to 10,
       the cycle length is 4.  Exponent modulo 4 suffices. */
    idx_t e = n % 4;
    seq_t pow_base = 1;
    seq_t b_val = base;
    while (e) {
        if (e & 1)
            pow_base = (pow_base * b_val) % 10;
        b_val = (b_val * b_val) % 10;
        e >>= 1;
    }

    return (mult * pow_base) % 10;
}
