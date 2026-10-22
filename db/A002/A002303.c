/*
 * A002303  Generalized tangent numbers.
 * https://oeis.org/A002303
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 64*(n+2)!/9! * (35*m^2 + 217*m + 306) with m = n-5, for n
 *            >= 7; a(4) = 16, a(5) = 272, a(6) = 3968. (From e.g.f.
 *            16*(17+78*t+45*t^2)/(1-t)^10 for a(n+5).)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 4) return (seq_t)16;
    if (n == 5) return (seq_t)272;
    if (n == 6) return (seq_t)3968;

    idx_t m = n - 5;
    seq_t sm = (seq_t)m;
    seq_t r = (seq_t)64 * ((seq_t)35 * sm * sm + (seq_t)217 * sm + (seq_t)306);
    for (idx_t i = 10; i <= m + 7; i++)
        r *= (seq_t)i;
    return r;
}
