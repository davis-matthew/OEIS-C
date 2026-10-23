/*
 * A008364  11-rough numbers: not divisible by 2, 3, 5 or 7.
 * https://oeis.org/A008364
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = base[(n-1)%48] + ((n-1)/48) * 210
 * Range:     u8 n <= 59, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static const seq_t base[48] = {
    (seq_t)1,  (seq_t)11, (seq_t)13, (seq_t)17, (seq_t)19, (seq_t)23, (seq_t)29, (seq_t)31,
    (seq_t)37, (seq_t)41, (seq_t)43, (seq_t)47, (seq_t)53, (seq_t)59, (seq_t)61, (seq_t)67,
    (seq_t)71, (seq_t)73, (seq_t)79, (seq_t)83, (seq_t)89, (seq_t)97, (seq_t)101, (seq_t)103,
    (seq_t)107, (seq_t)109, (seq_t)113, (seq_t)121, (seq_t)127, (seq_t)131, (seq_t)137, (seq_t)139,
    (seq_t)143, (seq_t)149, (seq_t)151, (seq_t)157, (seq_t)163, (seq_t)167, (seq_t)169, (seq_t)173,
    (seq_t)179, (seq_t)181, (seq_t)187, (seq_t)191, (seq_t)193, (seq_t)197, (seq_t)199, (seq_t)209
};

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;  /* sequence is 1‑indexed; undefined for n<1 */

    idx_t r = (n - 1) % 48;   /* 0‑based index into the base array */
    idx_t q = (n - 1) / 48;   /* number of 210‑steps to add */

    seq_t value = base[r];
    if (q != 0) {
        value += (seq_t)210 * (seq_t)q;
    }
    return value;
}
