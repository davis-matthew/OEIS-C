/*
 * A224923  a(n) = Sum_{i=0..n} Sum_{j=0..n} (i XOR j), where XOR is the
 *          binary logical exclusive-or operator.
 * https://oeis.org/A224923
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Sum_{i=0..n} Sum_{j=0..n} (i XOR j)
 * Range:     u8 n <= 7, u16 n <= 46, u32 n <= 1023, u64 n <= 1023,
 *            u128 n <= 1023, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t total = 0;
    seq_t nseq   = (seq_t)n;
    seq_t n_plus = nseq + 1;

    for (int b = 0; ; ++b)
    {
        seq_t mask = (seq_t)1 << b;
        if (mask > nseq)
            break;

        seq_t cycle_len = mask << 1;   /* 2^(b+1) */
        seq_t ones_cycle = mask;        /* 2^b */

        seq_t full_cycles = n_plus / cycle_len;
        seq_t ones_from_full = full_cycles * ones_cycle;

        seq_t rem = n_plus % cycle_len;
        seq_t ones_from_rem = (rem > ones_cycle) ? (rem - ones_cycle) : 0;

        seq_t ones_b  = ones_from_full + ones_from_rem;
        seq_t zeros_b = n_plus - ones_b;

        seq_t contrib = (seq_t)2 * mask * ones_b * zeros_b;
        total += contrib;
    }
    return total;
}
