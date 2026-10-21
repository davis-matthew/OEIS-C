/*
 * A230642  Number of integers m such that m + (sum of digits in base-3
 *          representation of m) = n.
 * https://oeis.org/A230642
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 331, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A230642: a(n) = #{ m ≥ 0 | m + sum_of_digits_base3(m) = n } */

static inline seq_t digit_sum_base3(seq_t x)
{
    seq_t sum = 0;
    while (x != 0)
    {
        sum += x % (seq_t)3;
        x   /= (seq_t)3;
    }
    return sum;
}

seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;
    seq_t count = 0;

    for (idx_t m = 0; m <= n; ++m)
    {
        seq_t m_seq = (seq_t)m;
        seq_t sum   = digit_sum_base3(m_seq);
        if (m_seq + sum == n_seq)
            count += 1;
    }

    return count;
}
