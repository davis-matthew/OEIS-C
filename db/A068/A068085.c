/*
 * A068085  Numbers k such that k and 10*k are both triangular numbers.
 * https://oeis.org/A068085
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1442*a(n-3) - a(n-6) + 99 for n > 3 with a(-2)=21,
 *            a(-1)=1, a(0)=0, a(1)=0, a(2)=1, a(3)=21 (OEIS offset 1)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 20, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t cache[10001];      /* holds values up to n = 10000 */
static idx_t cache_used = 0;    /* highest index computed so far */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* out of OEIS bounds, return 0 */

    /* extend cache up to n if necessary */
    for (idx_t i = cache_used + 1; i <= n; ++i)
    {
        if (i == 1)
            cache[i] = 0;
        else if (i == 2)
            cache[i] = 1;
        else if (i == 3)
            cache[i] = 21;
        else
        {
            /* recurrence: a(i) = 1442*a(i-3) - a(i-6) + 99 */
            seq_t term = (seq_t)1442 * cache[i - 3];
            int diff6 = (int)i - 6;          /* may be negative for i < 6 */
            if (diff6 >= 0)
                term = term - cache[diff6];
            else if (diff6 == -2)
                term = term - ((seq_t)21);
            else    /* diff6 == -1 */
                term = term - ((seq_t)1);
            term = term + ((seq_t)99);
            cache[i] = term;
        }
    }
    cache_used = n;
    return cache[n];
}
