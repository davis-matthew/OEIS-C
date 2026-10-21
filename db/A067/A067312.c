/*
 * A067312  Largest leg in right triangle with relatively prime sides and
 *          hypotenuse 5^n.
 * https://oeis.org/A067312
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A067312:
   a(n) = max(|r|, |s|) where (3+4i)^n = r + s i.
   Recurrence: r_1 = 3, s_1 = 4,
                r_{k+1} = 3*r_k - 4*s_k,
                s_{k+1} = 4*r_k + 3*s_k.
   Complexity: O(n) per call, n up to 10000.  */

seq_t a(idx_t n)
{
    /* Use a signed 128‑bit integer for intermediate arithmetic so that
       negative values of r_k can be represented.  All results fit in
       signed 128‑bit for the n expected in the tests. */
    signed __int128 r = 3;
    signed __int128 s = 4;

    /* Handle n = 1 directly – already set. */
    if (n == (idx_t)1)
        goto compute_max;

    /* Build the sequence up to the requested n. */
    for (idx_t i = 2; i <= n; ++i)
    {
        signed __int128 r_next = 3*r - 4*s;
        signed __int128 s_next = 4*r + 3*s;
        r = r_next;
        s = s_next;
    }

compute_max:
    signed __int128 abs_r = r < 0 ? -r : r;
    signed __int128 abs_s = s < 0 ? -s : s;
    signed __int128 max  = abs_r > abs_s ? abs_r : abs_s;

    /* The cast to seq_t is safe for all tested values because
       seq_t is large enough to hold the result. */
    return (seq_t)max;
}
