/*
 * A140130  a(n) = denominator(c(n)) where c(n) = 1 if n=1, otherwise if n <
 *          3*2^floor(log_2(n)-1) then c(n) =
 *          (c(floor(n/2))+c(floor((n+1)/2)))/2 otherwise c(n) =
 *          c(n-2^floor(log_2(n)))+1.
 * https://oeis.org/A140130
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator(c(n)) where c(1) = 1 and for n > 1 let k =
 *            floor(log2 n) if n < 3 * 2^(k‑1) c(n) = ( c(floor(n/2)) +
 *            c(floor((n+1)/2)) ) / 2 else c(n) = c(n - 2^k) + 1 The function
 *            returns the denominator of c(n)
 * Range:     u8 n <= 65, u16 n <= 1301, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

typedef struct { seq_t num; seq_t den; } fraction;

/* Euclidean algorithm for seq_t (works for integer or arbitrary‑precision seq_t) */
static inline seq_t gcd_seq_t(seq_t a, seq_t b)
{
    while (b != (seq_t)0) {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* floor(log2(v)) for v > 0, on idx_t (uint64_t) */
static inline unsigned int floor_log2_uint64(idx_t v)
{
    unsigned int r = 0;
    while (v >>= 1) ++r;
    return r;
}

/* Recursive computation returning fraction num/den of c(n) */
static fraction compute_c(idx_t n)
{
    if (n == 1) {
        fraction f = { (seq_t)1, (seq_t)1 };
        return f;
    }

    unsigned int k = floor_log2_uint64(n);
    idx_t threshold = (idx_t)3 << (k - 1);

    if (n < threshold) {
        idx_t a = n >> 1;
        idx_t b = (n + 1) >> 1;

        fraction fa = compute_c(a);
        fraction fb = compute_c(b);

        /* (fa.num/fa.den + fb.num/fb.den) / 2 */
        seq_t num_sum = fa.num * fb.den + fb.num * fa.den;
        seq_t den_prod = fa.den * fb.den;
        seq_t den_avg = den_prod * (seq_t)2;

        seq_t g = gcd_seq_t(num_sum, den_avg);
        fraction f = { num_sum / g, den_avg / g };
        return f;
    } else {
        idx_t shift = (idx_t)1 << k;
        idx_t m = n - shift;

        fraction fm = compute_c(m);

        /* fm.num / fm.den + 1 = (fm.num + fm.den) / fm.den */
        seq_t num_n = fm.num + fm.den;
        seq_t den_n = fm.den;

        seq_t g = gcd_seq_t(num_n, den_n);
        fraction f = { num_n / g, den_n / g };
        return f;
    }
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for offset 0, harmless */
    return compute_c(n).den;
}
