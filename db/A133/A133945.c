/*
 * A133945  Sum phi(k), where the sum is over the integers k which are the
 *          "isolated divisors" of n and phi(k) is the Euler totient function
 *          (phi(k) = A000010(k)). A positive divisor, k, of n is isolated if
 *          neither (k-1) nor (k+1) divides n.
 * https://oeis.org/A133945
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A133945: sum of Euler's totient values over isolated divisors of n.
   A divisor d of n is "isolated" if neither (d-1) nor (d+1) divides n.
   For odd n every divisor is isolated, so a(n)=n.
   For even n we use the identity Σ_{d|n} φ(d)=n and subtract the
   contributions of non‑isolated divisors. */
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

static uint64_t g_n;
static uint64_t g_primes[64];
static uint64_t g_exps[64];
static size_t g_pcnt;

/* Recursive generation of all divisors of g_n and accumulation of
   φ(d) for non‑isolated divisors. */
static void gen_divisors_rec(int idx, uint64_t cur_d, uint64_t cur_phi,
                             uint64_t *sum_nonisolated)
{
    if (idx == (int)g_pcnt) {
        /* Check isolation */
        bool isolated = true;
        if (cur_d > 1 && g_n % (cur_d - 1ULL) == 0ULL) isolated = false;
        if (cur_d < g_n && g_n % (cur_d + 1ULL) == 0ULL) isolated = false;
        if (!isolated) *sum_nonisolated += cur_phi;
        return;
    }

    uint64_t p   = g_primes[idx];
    uint64_t emax = g_exps[idx];
    uint64_t powp = 1ULL;                   /* p^e */
    for (uint64_t e = 0; e <= emax; ++e) {
        uint64_t new_d = cur_d * powp;
        uint64_t new_phi;
        if (e == 0) {
            new_phi = cur_phi;
        } else {
            /* φ(p^e) = p^(e-1)*(p-1) */
            new_phi = cur_phi * (powp / p) * (p - 1ULL);
        }
        gen_divisors_rec(idx + 1, new_d, new_phi, sum_nonisolated);
        powp *= p;
    }
}

seq_t a(idx_t n)
{
    /* All divisors of odd n are isolated: answer is n. */
    if ((n & 1ULL) == 1ULL) {
        return (seq_t)n;
    }

    /* Factor n into primes.  g_primes/g_exps/g_pcnt will be used by
       the recursive routine. */
    g_n = (uint64_t)n;
    uint64_t tmp = g_n;
    g_pcnt = 0;
    for (uint64_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0ULL) {
            uint64_t e = 0;
            while (tmp % p == 0ULL) {
                tmp /= p;
                ++e;
            }
            g_primes[g_pcnt] = p;
            g_exps[g_pcnt]   = e;
            ++g_pcnt;
        }
    }
    if (tmp > 1ULL) {
        g_primes[g_pcnt] = tmp;
        g_exps[g_pcnt]   = 1;
        ++g_pcnt;
    }

    uint64_t sum_nonisolated = 0;
    gen_divisors_rec(0, 1ULL, 1ULL, &sum_nonisolated);

    seq_t result = (seq_t)n - (seq_t)sum_nonisolated;
    return result;
}
