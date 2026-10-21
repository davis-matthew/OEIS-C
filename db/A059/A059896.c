/*
 * A059896  The set of Fermi-Dirac factors of A(n,k) is the union of the
 *          Fermi-Dirac factors of n and k. Symmetric square array read by
 *          antidiagonals.
 * https://oeis.org/A059896
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn, tabl
 * Formula:   a(n) is the (n‑th) entry of the symmetric square array A(i,j) =
 *            ∏ p^ (exp_i(p) OR exp_j(p)), read by antidiagonals. The
 *            coordinates (i,j) are obtained from the n-th index of the
 *            antidiagonal order
 * Range:     u8 n <= 508, u16 n <= 9995, u32 n <= 9995, u64 n <= 9995,
 *            u128 n <= 9995, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* --------------------------------------------------------------
   Utility: factorization by trial division (sufficient for
   the small integers encountered in this sequence).
   -------------------------------------------------------------- */
static void
factor_uint64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt)
{
    size_t c = 0;
    for (uint64_t p = 2; p * p <= n; ++p) {
        if (n % p == 0) {
            uint64_t t = 0;
            while (n % p == 0) {
                n /= p;
                ++t;
            }
            pr[c] = p;
            ex[c] = t;
            ++c;
        }
    }
    if (n > 1) {
        pr[c] = n;
        ex[c] = 1;
        ++c;
    }
    *cnt = c;
}

/* --------------------------------------------------------------
   Utility: exponentiation by squaring for arbitrary seq_t.
   -------------------------------------------------------------- */
static seq_t
pow_seq(seq_t base, seq_t exp)
{
    seq_t res = 1;
    while (exp > 0) {
        if (exp & 1)
            res *= base;
        base *= base;
        exp >>= 1;
    }
    return res;
}

/* --------------------------------------------------------------
   Main function: a(idx_t n)
   -------------------------------------------------------------- */
seq_t
a(idx_t n)
{
    /* Find the antidiagonal d such that d(d+1)/2 >= n. */
    idx_t d = 1;
    idx_t cum = 1;
    while (cum < n) {
        ++d;
        cum += d;
    }

    /* Compute coordinates (i,j) on the antidiagonal. */
    idx_t prev   = (d - 1) * d / 2;           /* sum up to previous diagonal */
    idx_t offset = n - prev - 1;              /* zero‑based position on this diagonal */
    idx_t i = offset + 1;
    idx_t j = d - offset;

    /* Factor i and j. */
    uint64_t pr_i[64], ex_i[64];
    uint64_t pr_j[64], ex_j[64];
    size_t cnt_i, cnt_j;

    factor_uint64((uint64_t)i, pr_i, ex_i, &cnt_i);
    factor_uint64((uint64_t)j, pr_j, ex_j, &cnt_j);

    /* Merge factorizations, using bitwise OR of exponents. */
    seq_t result = 1;
    size_t ai = 0, bj = 0;
    while (ai < cnt_i && bj < cnt_j) {
        if (pr_i[ai] == pr_j[bj]) {
            seq_t base = (seq_t)pr_i[ai];
            seq_t e1   = (seq_t)ex_i[ai];
            seq_t e2   = (seq_t)ex_j[bj];
            seq_t eor  = e1 | e2;
            result *= pow_seq(base, eor);
            ++ai; ++bj;
        } else if (pr_i[ai] < pr_j[bj]) {
            seq_t base = (seq_t)pr_i[ai];
            seq_t e    = (seq_t)ex_i[ai];
            result *= pow_seq(base, e);
            ++ai;
        } else {
            seq_t base = (seq_t)pr_j[bj];
            seq_t e    = (seq_t)ex_j[bj];
            result *= pow_seq(base, e);
            ++bj;
        }
    }
    while (ai < cnt_i) {
        seq_t base = (seq_t)pr_i[ai];
        seq_t e    = (seq_t)ex_i[ai];
        result *= pow_seq(base, e);
        ++ai;
    }
    while (bj < cnt_j) {
        seq_t base = (seq_t)pr_j[bj];
        seq_t e    = (seq_t)ex_j[bj];
        result *= pow_seq(base, e);
        ++bj;
    }

    return result;
}
