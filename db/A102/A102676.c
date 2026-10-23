/*
 * A102676  Number of digits >= 5 in the decimal representations of all
 *          integers from 0 to n.
 * https://oeis.org/A102676
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = Σ_{k≥0} Σ_{d=5}^{9} count of digit d at position 10^k in
 *            0..n
 * Range:     u8 n <= 269, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Counts the total number of decimal digits ≥ 5 appearing in the
   decimal representations of all integers from 0 to n inclusive.
   Uses the standard per‑digit counting technique. */
seq_t a(idx_t n)
{
    seq_t result = 0;
    idx_t power  = 1;                      /* 10^k */
    while (power <= n)
    {
        /* high : leading part above the current digit
           cur  : current digit at this position
           low  : trailing part below the current digit        */
        idx_t high = (n / power) / 10;     /* avoid overflow */
        int    cur  = (n / power) % 10;
        idx_t low  = n % power;

        seq_t sum_pos = 0;
        for (int d = 5; d <= 9; ++d)
        {
            seq_t cnt;
            if (cur > d)                     /* full block of this digit */
                cnt = (seq_t)(high + 1) * power;
            else if (cur == d)               /* partial block: digits < cur finished */
                cnt = (seq_t)high * power + (seq_t)low + 1;
            else                              /* cur < d: digit never reached */
                cnt = (seq_t)high * power;
            sum_pos += cnt;
        }
        result += sum_pos;

        /* Safely advance to the next power of 10 */
        if (power > n / 10) break;
        power *= 10;
    }
    return result;
}
