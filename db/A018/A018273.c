/*
 * A018273  Divisors of 76.
 * https://oeis.org/A018273
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Divisors of 76 – a(n) is the nth positive divisor of 76 in ascending order */
seq_t a(idx_t n)
{
    const seq_t target = (seq_t)76;
    seq_t divisors[20];
    unsigned cnt = 0;
    for(idx_t i = 1; ((seq_t)i * (seq_t)i) <= target; ++i) {
        if((target % (seq_t)i) == 0) {
            divisors[cnt++] = (seq_t)i;
            if(((seq_t)i * (seq_t)i) != target)
                divisors[cnt++] = target / (seq_t)i;
        }
    }
    for(unsigned i = 0; i < cnt; ++i)
        for(unsigned j = i + 1; j < cnt; ++j)
            if(divisors[j] < divisors[i]) {
                seq_t t = divisors[i];
                divisors[i] = divisors[j];
                divisors[j] = t;
            }
    if(n >= 1 && n <= cnt)
        return divisors[n - 1];
    return (seq_t)0;
}
