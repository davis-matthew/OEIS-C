/*
 * A171446  a(n) = 0+1+2+...+n in lunar arithmetic in base 3 written in base
 *          3.
 * https://oeis.org/A171446
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Σ_{p≥0} min(2, ⌊n/3^p⌋) * 10^p
 * Range:     u8 n <= 26, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) is the decimal value of the base‑3 digits obtained by
    taking the maximum digit seen in the interval 0..n for each
    position in base 3.  The digits are 0,1, or 2, and they are
    interpreted as decimal digits in the return value. */

seq_t a(idx_t n)
{
    seq_t result = 0;
    seq_t pow3   = 1;   /* 3^p, increases while pow3 <= n */
    seq_t pow10  = 1;   /* 10^p, weight for the decimal output */

    while (pow3 <= (seq_t)n) {
        idx_t t = n / (idx_t)pow3;          /* floor(n / 3^p) */
        seq_t digit = (t >= 2) ? (seq_t)2
                               : (t >= 1 ? (seq_t)1 : (seq_t)0);
        result += digit * pow10;

        /* prepare next power of 3; guard against overflow */
        if (pow3 > ((seq_t)n) / 3) break;
        pow3  *= (seq_t)3;
        pow10 *= (seq_t)10;
    }
    return result;
}
