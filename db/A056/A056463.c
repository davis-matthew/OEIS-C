/*
 * A056463  Number of primitive (aperiodic) palindromes using exactly two
 *          different symbols.
 * https://oeis.org/A056463
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{d|n} μ(d) * (2^{⌈(n/d)/2⌉} - 2) where μ is the Möbius
 *            function and ⌈x⌉ denotes the ceiling
 * Range:     u8 n <= 16, u16 n <= 32, u32 n <= 64, u64 n <= 128,
 *            u128 n <= 256, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* Maximum n to pre‑compute (enough for n≈10000 as required). */
#ifndef MAX_N
#define MAX_N 10000
#endif

/* Exponent needed for 2^e with e = ⌈(n/d)/2⌉ ≤ ⌈(MAX_N/1)/2⌉. */
#ifndef MAX_E
#define MAX_E 5000
#endif

/* Static state: Möbius table and powers of two. */
static bool state_initialized = false;
static int8_t mu_tbl[MAX_N + 1];      /* Möbius values: -1, 0, or +1 */
static seq_t pow2_tbl[MAX_E + 1];     /* pow2_tbl[e] = 2^e   */

/* Helper: initialize mu_tbl (linear sieve) and pow2_tbl. */
static void init_state(void)
{
    if (state_initialized) return;
    /* Compute Möbius function up to MAX_N */
    bool is_composite[MAX_N + 1] = {0};
    int primes[256], pcnt = 0;           /* enough for MAX_N =10000 */
    mu_tbl[1] = 1;
    for (idx_t i = 2; i <= MAX_N; ++i) {
        if (!is_composite[i]) {
            primes[pcnt++] = (int)i;
            mu_tbl[(int)i] = -1;          /* prime -> μ = -1 */
        }
        for (idx_t j = 0; j < pcnt; ++j) {
            idx_t p = (idx_t)primes[j];
            idx_t ip = i * p;
            if (ip > MAX_N) break;
            is_composite[ip] = true;
            if (i % p == 0) {
                mu_tbl[(int)ip] = 0;      /* square factor -> μ = 0 */
                break;
            } else {
                mu_tbl[(int)ip] = -mu_tbl[(int)i];
            }
        }
    }

    /* Pre‑compute powers of two: pow2_tbl[e] = 2^e */
    pow2_tbl[0] = (seq_t)1;
    for (idx_t i = 1; i <= MAX_E; ++i) {
        pow2_tbl[i] = pow2_tbl[i-1] * (seq_t)2;
    }

    state_initialized = true;
}

/* Main function required by the specification. */
seq_t a(idx_t n)
{
    init_state();                 /* lazy static initialization */

    /* Enumerate all divisors of n */
    seq_t sum_pos = 0, sum_neg = 0;   /* keep positives and negatives separate */

    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d != 0) continue;

        /* Process divisor d */
        int mu_val = mu_tbl[(int)d];
        if (mu_val != 0) {
            idx_t q = n / d;                     /* n/d */
            idx_t ceil_half = (q + 1) / 2;        /* ⌈(n/d)/2⌉ */
            seq_t term = pow2_tbl[ceil_half] - (seq_t)2;  /* 2^{ceil_half} - 2 */

            if (mu_val == 1) sum_pos += term;
            else /* mu_val == -1 */ sum_neg += term;
        }

        /* Process paired divisor n/d if distinct */
        idx_t d2 = n / d;
        if (d2 != d) {
            int mu_val2 = mu_tbl[(int)d2];
            if (mu_val2 != 0) {
                idx_t q = n / d2;                 /* n/(n/d) = d */
                idx_t ceil_half = (q + 1) / 2;    /* ⌈d/2⌉ */
                seq_t term = pow2_tbl[ceil_half] - (seq_t)2;

                if (mu_val2 == 1) sum_pos += term;
                else /* mu_val2 == -1 */ sum_neg += term;
            }
        }
    }

    /* The result is non‑negative: sum_pos ≥ sum_neg */
    return sum_pos - sum_neg;
}
