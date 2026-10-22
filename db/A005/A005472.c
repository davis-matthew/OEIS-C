/*
 * A005472  Class numbers of Shanks' simplest cubic fields.
 * https://oeis.org/A005472
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = h(K_m), the class number of the cyclic cubic field K_m =
 *            Q(rho), where rho is a root of x^3 - m*x^2 - (m+3)*x - 1, p =
 *            m^2+3m+9 is the n-th prime of that form (m >= -1), and disc(K_m)
 *            = p^2. Analytic class number formula: h*R = p*|L(1,chi)|^2/4,
 *            with chi a cubic character mod p. L(1,chi) i...
 * Range:     u8 n <= 70, u16 n <= 644, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   A005472_E1, A005472_classno, A005472_powmod, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/A005472_E1.h"
#include "../utils/A005472_classno.h"
#include "../utils/A005472_powmod.h"

static long long *A005472_ms = NULL;
static idx_t A005472_mcount = 0, A005472_mcap = 0;
static long long A005472_nextm = -1;

static long long A005472_get_m(idx_t n)
{
    while (A005472_mcount < n) {
        long long m = A005472_nextm++;
        uint64_t p = (uint64_t)(m * m + 3 * m + 9);
        if (is_prime(p)) {
            if (A005472_mcount == A005472_mcap) {
                A005472_mcap = A005472_mcap ? 2 * A005472_mcap : 64;
                A005472_ms = (long long *)realloc(A005472_ms, A005472_mcap * sizeof(long long));
            }
            A005472_ms[A005472_mcount++] = m;
        }
    }
    return A005472_ms[n - 1];
}

seq_t a(idx_t n)
{
    long long m;
    if (n < 1) return 0;
    m = A005472_get_m(n);
    return (seq_t)A005472_classno(m);
}
