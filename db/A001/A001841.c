/*
 * A001841  Related to Zarankiewicz's problem.
 * https://oeis.org/A001841
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*(n-1)/2 if n is odd, else n*(n-1)/2 - ceil(n/6)
 * Range:     u8 n <= 23, u16 n <= 362, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t base = (seq_t)n * ((seq_t)n - 1) / 2;
    
    if (n % 2 == 1) {
        return base;
    } else {
        seq_t reduction = ((seq_t)n + 5) / 6;
        return base - reduction;
    }
}
