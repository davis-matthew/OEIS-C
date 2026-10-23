/*
 * A136445  Size of the BDD for the hidden weighted bit function, with the
 *          variables in their natural ordering.
 * https://oeis.org/A136445
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 31, u32 n <= 70, u64 n <= 149,
 *            u128 n <= 307, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Size of the BDD for the hidden weighted bit function,
   with the variables in their natural ordering.
   It satisfies the recurrence:
     a(1)=3, a(2)=3, a(3)=7, a(4)=10, a(5)=17, a(6)=25,
     a(7)=40, a(8)=57, a(9)=85
     For n>9:
     a(n) = a(n-1) + 2*a(n-2) - 3*a(n-4) - 2*a(n-5)
            + 2*a(n-6) + 2*a(n-7) - a(n-9).
*/

static seq_t seq[10001] = {0};
static int initialized = 0;
static idx_t max_computed = 9;

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n < 1 */
    if (!initialized)
    {
        seq[1] = 3;
        seq[2] = 3;
        seq[3] = 7;
        seq[4] = 10;
        seq[5] = 17;
        seq[6] = 25;
        seq[7] = 40;
        seq[8] = 57;
        seq[9] = 85;
        max_computed = 9;
        initialized = 1;
    }
    if (n <= max_computed) return seq[n];

    for (idx_t i = max_computed + 1; i <= n; ++i)
    {
        seq_t pos = seq[i-1] + seq[i-2]*2 + seq[i-6]*2 + seq[i-7]*2;
        seq_t neg = seq[i-4]*3 + seq[i-5]*2 + seq[i-9];
        seq[i]   = pos - neg;
    }
    max_computed = n;
    return seq[n];
}
