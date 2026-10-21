/*
 * A057430  Number of days in months of Baha'i calendar.
 * https://oeis.org/A057430
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 19 for 1 ≤ n ≤ 19 and for 21 ≤ n ≤ 39, …; a(20 + 20*k) =
 *            5 if k ≡ 0 (mod 4), otherwise 4. In words: every 20th month (the
 *            intercalary month) has 4 days, except every 4th year (counting
 *            the first year as year 0) where it has 5 days
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* number of days in Baha'i calendar month n,  n ≥ 1  */
seq_t a(idx_t n)
{
    /* year index starting from 0 (first year has index 0)   */
    idx_t year = (n - 1) / 20;
    /* 0‑based position within the year’s 20 months   */
    idx_t pos  = (n - 1) % 20;

    /* intercalary month is the 20th month of the year   */
    if (pos == 19) {
        /* leap years: year numbers ≡ 0 (mod 4)    */
        return (year % 4 == 0) ? 5 : 4;
    }
    return 19;
}
