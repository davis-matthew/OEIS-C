/*
 * A184917  n+[rn/s]+[tn/s]+[un/s], where []=floor and r=1, s=2^(1/4), t=s^2,
 *          u=s^3.
 * https://oeis.org/A184917
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + ⌊n/s⌋ + ⌊s·n⌋ + ⌊s²·n⌋ * where s = 2^(1/4). The
 *            implementation uses only integer * arithmetic. ⌊s²·n⌋ is ⌊√2 ·
 *            n⌋, which is obtained by an * exact integer square root. ⌊s·n⌋
 *            and ⌊n/s⌋ are computed * via a fourth‑root: (s·n)⁴ = 2 · n⁴ and
 *            (n/s)⁴ = n⁴/2
 * Range:     u8 n <= 3, u16 n <= 13, u32 n <= 120, u64 n <= 120,
 *            u128 n <= 120, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t floor_sqrt(seq_t x)
{
    /* exact integer floor of sqrt(x) by binary search */
    seq_t low  = 0;
    seq_t high = ((seq_t)1) << (sizeof(seq_t)*8/2);  /* 2^(bits/2) >= sqrt(max) */
    while (low < high) {
        seq_t mid = (low + high + 1) >> 1;
        unsigned __int128 mid2 = (unsigned __int128)mid * (unsigned __int128)mid;
        if (mid2 <= x)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

static seq_t floor_fourth_root(seq_t x)
{
    /* exact integer floor of x^(1/4) by binary search */
    seq_t low  = 0;
    seq_t high = ((seq_t)1) << (sizeof(seq_t)*8/4);  /* 2^(bits/4) >= fourth_root(max) */
    while (low < high) {
        seq_t mid = (low + high + 1) >> 1;
        unsigned __int128 mid2 = (unsigned __int128)mid * (unsigned __int128)mid;
        unsigned __int128 mid4 = mid2 * mid2;
        if (mid4 <= x)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;

    /* ⌊s²·n⌋ with s² = √2  */
    seq_t sqrt_input = (seq_t)((unsigned __int128)2 * nn * nn);
    seq_t sqrt_term  = floor_sqrt(sqrt_input);

    /* compute n⁴ */
    seq_t n2 = nn * nn;
    seq_t n4 = n2 * n2;

    /* ⌊s·n⌋  :  (s·n)⁴ = 2 n⁴  */
    seq_t alpha_term = floor_fourth_root((seq_t)((unsigned __int128)2 * n4));

    /* ⌊n/s⌋ :  (n/s)⁴ = n⁴ / 2  */
    seq_t inv_alpha_term = floor_fourth_root(n4 >> 1);  /* integer division by 2 */

    return nn + inv_alpha_term + alpha_term + sqrt_term;
}
