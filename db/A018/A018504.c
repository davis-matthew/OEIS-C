/*
 * A018504  Divisors of 531.
 * https://oeis.org/A018504
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = nth divisor of 531 (sorted ascending)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static seq_t divisors[6];
static bool initialized = false;

static seq_t pow_seq_int(seq_t base, int exp)
{
    seq_t res = 1;
    for (int i = 0; i < exp; ++i)
        res *= base;
    return res;
}

static void init_divisors(void)
{
    if (initialized) return;
    /* prime factorization of 531: 3^2 * 59^1 */
    const seq_t primes[2] = {3, 59};
    const int exps[2] = {2, 1};
    int idx = 0;
    /* generate all combinations of exponents */
    for (int e0 = 0; e0 <= exps[0]; ++e0) {
        seq_t v0 = pow_seq_int(primes[0], e0);
        for (int e1 = 0; e1 <= exps[1]; ++e1) {
            seq_t v1 = pow_seq_int(primes[1], e1);
            divisors[idx++] = v0 * v1;
        }
    }
    /* sort ascending – trivial bubble sort for 6 elements */
    for (int i = 0; i < 5; ++i) {
        for (int j = i + 1; j < 6; ++j) {
            if (divisors[i] > divisors[j]) {
                seq_t tmp = divisors[i];
                divisors[i] = divisors[j];
                divisors[j] = tmp;
            }
        }
    }
    initialized = true;
}

seq_t a(idx_t n)
{
    /* sequence defined for n >= 1; for n > 6 we return 0 */
    if (n == 0) return 0;  /* not used by the OEIS offset */
    init_divisors();
    if (n <= 6)
        return divisors[(idx_t)n - 1];
    return 0;
}
