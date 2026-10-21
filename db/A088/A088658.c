/*
 * A088658  Number of triangles in an n X n unit grid that have minimal
 *          possible area (of 1/2).
 * https://oeis.org/A088658
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*(n-1)^2 + 4*Σ_{i=2}^{n-1} (n-i)*(2n-i)*phi(i)
 * Range:     u8 n <= 4, u16 n <= 18, u32 n <= 289, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static idx_t phi_max = 0;
static unsigned long long *phi_arr = NULL;

/* Ensure phi values are available up to max_value inclusive */
static void ensure_phi(idx_t max_value)
{
    if (max_value <= phi_max)
        return;

    /* Allocate or grow the array */
    unsigned long long *new_arr = (unsigned long long *)
        realloc(phi_arr, (max_value + 1) * sizeof(unsigned long long));
    if (!new_arr) {
        /* Allocation failed – abort */
        return;
    }
    phi_arr = new_arr;

    /* Compute phi for all values up to max_value */
    for (idx_t i = 0; i <= max_value; ++i)
        phi_arr[i] = i;
    for (idx_t i = 2; i <= max_value; ++i) {
        if (phi_arr[i] == i) { /* i is prime */
            for (idx_t j = i; j <= max_value; j += i)
                phi_arr[j] -= phi_arr[j] / i;
        }
    }
    phi_max = max_value;
}

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)0;

    /* Precompute phi up to n-1 */
    ensure_phi(n - 1);

    seq_t sum = 0;
    for (idx_t i = 2; i <= n - 1; ++i) {
        seq_t term = (seq_t)(n - i) * (seq_t)(2 * n - i);
        term *= (seq_t)phi_arr[i];
        sum += term;
    }

    seq_t base = (seq_t)(n - 1) * (seq_t)(n - 1) + sum;
    seq_t result = 4 * base;
    return result;
}
