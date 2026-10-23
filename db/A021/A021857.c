/*
 * A021857  Decimal expansion of 1/853.
 * https://oeis.org/A021857
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   not stated
 * Range:     u8 n <= 0, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Digit of the decimal expansion of 1/853.
   Returns the nth digit (0‑indexed) of 1/853 in base 10.
   Implemented by simulating long division. */
seq_t a(idx_t n)
{
    /* 1/853 has period 852 (since 853 is prime and 10 is primitive), so
       any digit can be obtained by iterating the remainder 852 times,
       but we simply iterate to n as n is at most ~10^4. */
    seq_t rem = 1;   /* initial remainder after leading "0." */
    seq_t digit = 0;
    const seq_t div = (seq_t)853;
    idx_t i;
    for (i = 0; i <= n; ++i) {
        rem *= (seq_t)10;           /* bring down next zero */
        digit = rem / div;          /* extract next digit */
        rem -= digit * div;         /* remainder for next step */
    }
    return digit;
}
