/*
 * A021593  Decimal expansion of 1/589.
 * https://oeis.org/A021593
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = floor( ((10^n mod 589) * 10) / 589 ) * The function
 *            returns the nth digit after the decimal point of 1/589, * with
 *            a(0) being the first such digit
 * Range:     u8 n <= 0, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t MOD = 589;   /* the denominator 589 */
    idx_t     exp = n;              /* exponent for 10^n */
    seq_t     base = 10;            /* current power of 10 modulo MOD */
    seq_t     rem  = 1 % MOD;       /* remainder = 10^0 mod MOD */

    /* fast modular exponentiation: compute rem = 10^n mod MOD */
    while (exp) {
        if (exp & 1u)
            rem = (rem * base) % MOD;
        base = (base * base) % MOD;
        exp >>= 1;
    }

    /* The nth decimal digit is (rem * 10) / MOD.  rem < MOD, so
     * Python: digit = floor((rem*10)/589).  The quotient is an
     * integer 0‑9, fitting in seq_t. */
    seq_t digit = (rem * (seq_t)10) / MOD;
    return digit;
}
