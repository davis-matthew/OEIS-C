/*
 * A239907  Let cn(n,k) denote the number of times 11..1 (k 1's) appears in
 *          the binary representation of n; a(n) = n - cn(n,1) + cn(n,2) -
 *          cn(n,3) + cn(n,4) - ... .
 * https://oeis.org/A239907
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n - \sum_{\text{runs of consecutive 1s in }n} \lceil
 *            \text{length}/2 \rceil
 * Range:     u8 n <= 257, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;          /* sum of ceil(len/2) for each run of 1 bits */
    int run_len = 0;        /* current run length of consecutive 1 bits */
    idx_t v = n;

    while (v != 0) {
        if (v & 1U) {
            run_len++;
        } else {
            if (run_len > 0) {
                sum += ((seq_t)run_len + 1) / 2;
                run_len = 0;
            }
        }
        v >>= 1;
    }
    if (run_len > 0) {
        sum += ((seq_t)run_len + 1) / 2;
    }

    return ((seq_t)n) - sum;
}
