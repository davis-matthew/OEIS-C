/*
 * A224473  a(n) = (2*5^(2^n) - 1) mod 10^n: a sequence of trimorphic numbers
 *          ending in 9.
 * https://oeis.org/A224473
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = (2*5^(2^n) - 1) mod 10^n
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation builds the integer digit‑by‑digit by solving the two
   congruence conditions:
     a(n) ≡ 1 (mod 2^n)     and
     a(n) ≡ -1 (mod 5^n)
   The unique digit 0–9 that extends a(n‑1) to a(n) is found by simple
   brute‑force testing, which is fast because only a few small
   arithmetic operations are performed per step. */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0)            /* index below offset, value irrelevant */
        return (seq_t)0;

    seq_t result = (seq_t)9;          /* a(1) */
    if (n == 1)
        return result;

    /* Current power of 2 and 5, starting with 2^1 and 5^1. */
    seq_t mod2 = (seq_t)2;
    seq_t mod5 = (seq_t)5;

    /* 10^k % mod2 and mod5 for the next step */
    seq_t ten_mod2 = (seq_t)1;    /* 10^0 mod 2^1 */
    seq_t ten_mod5 = (seq_t)1;    /* 10^0 mod 5^1 */

    /* 10^k as a big integer, used for appending the new digit */
    seq_t pow10_big = (seq_t)10;  /* 10^1, for cur = 2 */

    for (idx_t cur = 2; cur <= n; ++cur) {
        /* Update modulus values: 2^cur and 5^cur. */
        mod2 = mod2 * (seq_t)2;
        mod5 = mod5 * (seq_t)5;

        /* Update 10^{cur-1} modulo the new moduli. */
        ten_mod2 = (ten_mod2 * (seq_t)10) % mod2;
        ten_mod5 = (ten_mod5 * (seq_t)10) % mod5;

        /* Remainders of the current result modulo the new moduli. */
        seq_t rem2 = result % mod2;
        seq_t rem5 = result % mod5;

        /* Find the single digit d (0‑9) satisfying both congruences. */
        int digit = -1;
        for (int d = 0; d <= 9; ++d) {
            seq_t a2 = (ten_mod2 * (seq_t)d + rem2) % mod2;
            if (a2 != (seq_t)1)
                continue;
            seq_t a5 = (ten_mod5 * (seq_t)d + rem5) % mod5;
            if (a5 == mod5 - (seq_t)1) {
                digit = d;
                break;
            }
        }

        /* Append the digit: new result = digit * 10^{cur-1} + previous. */
        result = result + (pow10_big * (seq_t)digit);

        /* Prepare 10^cur for the next iteration. */
        pow10_big = pow10_big * (seq_t)10;
    }

    return result;
}
