/*
 * A034104  Fractional part of square root of a(n) starts with digit 8.
 * https://oeis.org/A034104
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = the n‑th integer k whose fractional part of sqrt(k)
 *            starts with digit 8 (base 10)
 * Range:     u8 none, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is 1‑based. */
    if (n == 0)
        return 0;   /* safety guard, the sequence starts at n = 1 */

    seq_t target = (seq_t)n;
    seq_t cum = 0;   /* count of terms already seen */

    for (idx_t m = 0; ; ++m)
    {
        seq_t m_seq = (seq_t)m;

        /* (m + 0.8)^2 = (25 m^2 + 40 m + 16) / 25 */
        seq_t num_lower = (seq_t)25 * m_seq * m_seq + (seq_t)40 * m_seq + (seq_t)16;
        seq_t lower = (num_lower + (seq_t)24) / (seq_t)25;   /* ceil division */

        /* (m + 0.9)^2 = (100 m^2 + 180 m + 81) / 100 */
        seq_t num_upper = (seq_t)100 * m_seq * m_seq + (seq_t)180 * m_seq + (seq_t)81;
        seq_t upper = num_upper / (seq_t)100;                /* floor division */

        if (upper < lower)
            continue;   /* no integer in this interval */

        seq_t count = upper - lower + (seq_t)1;   /* number of integers in this interval */

        if (cum + count >= target)              /* desired term lies here */
        {
            seq_t offset = target - cum - (seq_t)1; /* zero‑based index within interval */
            return lower + offset;
        }

        cum += count;   /* move past this interval */
    }
}
