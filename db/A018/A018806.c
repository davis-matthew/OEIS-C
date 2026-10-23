/*
 * A018806  Sum of gcd(x, y) for 1 <= x, y <= n.
 * https://oeis.org/A018806
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1..n} φ(k) * (⌊n/k⌋)^2 (sum of gcd over 1..n)
 * Range:     u8 n <= 11, u16 n <= 141, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

static seq_t *phi = NULL;
static idx_t phi_max = 0;

/* ensure φ(i) is computed for all 1 ≤ i ≤ n */
static void update_phi(idx_t n)
{
    if (n <= phi_max) return;

    seq_t *new_phi = alloc_seq_array(n + 1);
    if (!new_phi) return;          /* allocation failed – return with phi unchanged */

    /* initialize φ(i) = i */
    for (idx_t i = 0; i <= n; ++i) {
        new_phi[i] = (seq_t)i;
    }

    /* linear sieve to compute φ */
    for (idx_t p = 2; p <= n; ++p) {
        if (new_phi[p] == (seq_t)p) {          /* p is prime */
            for (idx_t j = p; j <= n; j += p) {
                new_phi[j] = new_phi[j] - new_phi[j] / (seq_t)p;
            }
        }
    }

    if (phi) free_seq_array(phi);
    phi = new_phi;
    phi_max = n;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    update_phi(n);

    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        idx_t q = n / k;
        seq_t term = phi[k] * (seq_t)q * (seq_t)q;
        sum += term;
    }
    return sum;
}
