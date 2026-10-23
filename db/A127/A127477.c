/*
 * A127477  Triangle T(n,k) read by rows: matrix product A054522 * A054523.
 * https://oeis.org/A127477
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) is the n‑th term of the triangle T read by rows. T(r,k) = 0
 *            if k∤r; otherwise T(r,k) = Σ_{d | r/k} φ(d) * φ(k·d) where φ is
 *            Euler’s totient function
 * Range:     u8 n <= 94, u16 n <= 94, u32 n <= 94, u64 n <= 94, u128 n <= 94,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <math.h>

/*  Ensure that the array phi[] contains φ(x) for 0 ≤ x ≤ N.  */
static seq_t *phi = NULL;
static idx_t phi_max = 0;

static void ensure_phi(idx_t N)
{
    if (N <= phi_max)
        return;

    /* Reallocate and recompute φ up to N using a linear sieve. */
    seq_t *new_phi = (seq_t *)realloc(phi, (N + 1) * sizeof(seq_t));
    if (!new_phi) exit(1);
    phi = new_phi;

    /* Use a simple linear sieve. */
    idx_t *is_comp = (idx_t *)calloc((size_t)(N + 1), sizeof(idx_t));
    if (!is_comp) exit(1);

    /* Allocate space for primes.  Rough estimate: N/10 + 10. */
    size_t primes_cap = (N / 10 + 10);
    idx_t *primes = (idx_t *)malloc(primes_cap * sizeof(idx_t));
    if (!primes) {
        free(is_comp);
        exit(1);
    }
    size_t primes_cnt = 0;

    phi[0] = 0;
    phi[1] = 1;

    for (idx_t i = 2; i <= N; ++i) {
        if (!is_comp[i]) {
            primes[primes_cnt++] = i;
            phi[i] = (seq_t)(i - 1);
        }
        for (size_t pi = 0; pi < primes_cnt; ++pi) {
            idx_t p = primes[pi];
            idx_t ip = i * p;
            if (ip > N) break;
            is_comp[ip] = 1;
            if (i % p == 0) {
                phi[ip] = phi[i] * (seq_t)p;
                break;
            } else {
                phi[ip] = phi[i] * (seq_t)(p - 1);
            }
        }
    }
    free(is_comp);
    free(primes);
    phi_max = N;
}

/*  Compute T(row, col) using the divisor sum formula.  */
static seq_t compute_T(idx_t row, idx_t col)
{
    if (row % col != 0)
        return (seq_t)0;

    idx_t m = row / col;

    /* Ensure φ values are available up to 'row'. */
    if (row > phi_max)
        ensure_phi(row);

    seq_t sum = 0;
    for (idx_t d = 1; d * d <= m; ++d) {
        if (m % d == 0) {
            idx_t d1 = d;
            idx_t d2 = m / d;

            seq_t phi_d1  = phi[d1];
            seq_t phi_col_d1 = phi[col * d1];
            sum += phi_d1 * phi_col_d1;

            if (d1 != d2) {
                seq_t phi_d2  = phi[d2];
                seq_t phi_col_d2 = phi[col * d2];
                sum += phi_d2 * phi_col_d2;
            }
        }
    }
    return sum;
}

/*  Public function required by the build system.  */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* Offset 1; below offset is undefined. */

    /* Determine row r and column k from the global index n. */
    double t = sqrt(8.0 * (double)n - 7.0);          /* 8n-7 fits in double. */
    idx_t r = (idx_t)((t + 1.0) / 2.0);              /* floor((sqrt(8n-7)+1)/2) */
    idx_t offset = ((r - 1) * r) / 2;                /* terms before row r */
    idx_t k = n - offset;                            /* 1‑based column within row */

    return compute_T(r, k);
}
