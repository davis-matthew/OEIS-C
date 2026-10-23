/*
 * A169681  Decimal expansion of 2^127-1.
 * https://oeis.org/A169681
 *
 * Offset:    39    (a(39) is the first term)
 * Keywords:  nonn, cons, easy, fini, full
 * Formula:   a(n) is the (n‑38)-th decimal digit of 2^127‑1
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Decimal expansion of 2^127-1:
       170141183460469231731687303715884105727   (39 digits)
       OEIS offset = 39 → a(39) is the first digit `1`. */
    static const char *digits =
        "170141183460469231731687303715884105727";
    const idx_t offset = 39;

    if (n < offset || (n - offset) >= 39)
        return (seq_t)0;                // undefined out of range

    return (seq_t)(digits[n - offset] - '0');
}
