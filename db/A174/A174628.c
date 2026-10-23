/*
 * A174628  "Binary dates": take the dates with the format dd/mm/yy that have
 *          only 0's and 1's and transform their value to base 10.
 * https://oeis.org/A174628
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, fini, full, nonn
 * Formula:   a(n) = the decimal value of the n‑th binary date dd/mm/yy
 *            (digits only 0 or 1) interpreted as a 6‑bit binary number,
 *            ordered by year, month, then day
 * Range:     u8 n <= 36, u16 n <= 36, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* sequence is defined only for the 36 dates with digits 0/1 */
    if (n == 0 || n > 36)
        return 0;               /* undefined for other indices */

    idx_t k = n - 1;                 /* zero‑based index */

    /* year, month, day indices:
       day index ∈ {0,1,2}  (days 01,10,11)
       month index ∈ {0,1,2} (months 01,10,11)
       year index ∈ {0,1,2,3} (years 00,01,10,11) */
    idx_t year  = k / 9;                /* 0 .. 3 */
    idx_t month = (k % 9) / 3;          /* 0 .. 2 */
    idx_t day   = k % 3;                /* 0 .. 2 */

    /* Convert indices to 2‑bit binary values.
       Month and day are {01,10,11} → values 1,2,3.
       Year is {00,01,10,11} → values 0,1,2,3. */
    uint8_t day_val   = (uint8_t)(day + 1);      /* 1..3 */
    uint8_t month_val = (uint8_t)(month + 1);    /* 1..3 */
    uint8_t year_val  = (uint8_t)(year & 3);     /* 0..3 */

    /* Build the 6‑bit binary number:
       bits: dd(mm)(yy) → d_high d_low m_high m_low y_high y_low */
    seq_t val = 0;
    val |= ((seq_t)((day_val   >> 1) & 1)) << 5;
    val |= ((seq_t)(day_val          & 1)) << 4;
    val |= ((seq_t)((month_val >> 1) & 1)) << 3;
    val |= ((seq_t)(month_val          & 1)) << 2;
    val |= ((seq_t)((year_val  >> 1) & 1)) << 1;
    val |= ((seq_t)(year_val           & 1));

    return val;
}
