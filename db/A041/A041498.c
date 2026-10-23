/*
 * A041498  Numerators of continued fraction convergents to sqrt(266).
 * https://oeis.org/A041498
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 11, u64 n <= 23, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numerators of the continued fraction convergents to sqrt(266).
   a(0)=16, a(1)=49; for n≥2, a(n)=a_k·a(n‑1)+a(n‑2),
   where a_k = 3, 4, 3, 32 repeat for k≥1 (partial quotients
   of the continued fraction of sqrt(266)). */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)16;
    if (n == 1)
        return (seq_t)49;

    seq_t p_nm2 = (seq_t)16; /* p_{n-2} */
    seq_t p_nm1 = (seq_t)49; /* p_{n-1} */

    for (idx_t i = 2; i <= n; ++i)
    {
        int a_k;
        switch ((i - 1) % 4)
        {
            case 0: a_k = 3;  break;
            case 1: a_k = 4;  break;
            case 2: a_k = 3;  break;
            default: a_k = 32; break;
        }
        seq_t p_i = (seq_t)a_k * p_nm1 + p_nm2;
        p_nm2 = p_nm1;
        p_nm1 = p_i;
    }
    return p_nm1;
}
