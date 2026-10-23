/*
 * A132031  Product{0<=k<=floor(log_7(n)), floor(n/7^k)}, n>=1.
 * https://oeis.org/A132031
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{k=0}^{⌊log_7 n⌋} ⌊ n / 7^k ⌋, n ≥ 1
 * Range:     u8 n <= 42, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t prod = 1;
    idx_t power = 1;

    while (1) {
        prod *= (seq_t)(n / power);
        if (power > n / 7)
            break;
        power *= 7;
    }

    return prod;
}
