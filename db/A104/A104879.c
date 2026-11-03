/*
 * A104879  Row sums of a sum-of-powers triangle.
 * https://oeis.org/A104879
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 1 + n + Sum_{k=2..n+1} (k^(n-k+1)-1)/(k-1)
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 17, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Validated: partial, pass, 27 terms through n = 26 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t sum_powers_recursive(seq_t base, idx_t exp, seq_t *pow_out)
{
    /* Returns Σ_{i=0}^{exp-1} base^i.
       *pow_out receives base^exp.  (exp ≥ 0) */
    if (exp == 0) {
        *pow_out = (seq_t)1;
        return (seq_t)0;
    }
    if (exp == 1) {
        *pow_out = base;
        return (seq_t)1;
    }
    if ((exp & 1) == 0) {          /* even exp */
        seq_t pow_half;
        seq_t sum_half = sum_powers_recursive(base, exp >> 1, &pow_half);
        seq_t pow_new = pow_half * pow_half;      /* base^exp */
        *pow_out = pow_new;
        return sum_half * (seq_t)(1 + pow_half);
    } else {                       /* odd exp */
        seq_t pow_prev;
        seq_t sum_prev = sum_powers_recursive(base, exp - 1, &pow_prev);
        seq_t sum_new = sum_prev + pow_prev;      /* base^(exp-1) added */
        seq_t pow_new = pow_prev * base;          /* base^exp */
        *pow_out = pow_new;
        return sum_new;
    }
}

static seq_t sum_powers(seq_t base, idx_t exp)
{
    seq_t pow_val;
    return sum_powers_recursive(base, exp, &pow_val);
}
/* This implements the sum of powers in a recursive, O(n log n) manner. */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    seq_t total = (seq_t)0;
    for (idx_t k = 2; k <= n; ++k) {
        idx_t exp = n - k + 1;         /* exp ≥ 1 for k ≤ n */
        seq_t sum_k = sum_powers((seq_t)k, exp);
        total += sum_k;
    }
    return (seq_t)1 + (seq_t)n + total;
}
