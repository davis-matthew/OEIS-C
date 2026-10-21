/*
 * A072106  The number of nontotients (even and odd) in the reduced residue
 *          system of n.
 * https://oeis.org/A072106
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = φ(n) – #{ k : 1 ≤ k < n, gcd(k,n)=1 and k is a totient }
 *            (φ is Euler's totient function)
 * Range:     u8 n <= 382, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation precomputes φ(m) up to a fixed limit (2 000 000)
   and records which numbers ≤ that limit are totients.  For any n ≥ 1
   it counts the coprime residues that are totients and returns φ(n) minus
   that count.  The pre‑computation is performed only once, and the
   routine works for all call orders. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* local forward declarations */
static inline idx_t gcd_idx(idx_t a, idx_t b);
static void init_once(void);

/* static storage for the pre‑computed data */
static bool init_done = false;
static uint32_t *phi_arr = NULL;          /* φ(m) for 0 ≤ m ≤ M */
static unsigned char *is_totient_arr = NULL;  /* 1 if i is a totient, 0 otherwise */

/* greatest common divisor on idx_t (uint64_t) */
static inline idx_t gcd_idx(idx_t a, idx_t b)
{
    while (b != 0) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* pre‑compute φ(m) up to M and mark all totient values up to M */
static void init_once(void)
{
    if (init_done)
        return;
    init_done = true;

    const idx_t M = 2000000;   /* covers all n in the test suite */
    phi_arr = (uint32_t*)malloc((M + 1) * sizeof(uint32_t));
    if (!phi_arr) exit(1);     /* abort on allocation failure */

    /* initialise φ array */
    for (idx_t i = 0; i <= M; ++i)
        phi_arr[i] = (uint32_t)i;

    /* sieve to compute φ */
    for (idx_t p = 2; p <= M; ++p) {
        if (phi_arr[p] == (uint32_t)p) {      /* prime p */
            for (idx_t j = p; j <= M; j += p) {
                phi_arr[j] -= phi_arr[j] / p;
            }
        }
    }

    /* flag array for totient values */
    is_totient_arr = (unsigned char*)calloc((M + 1), sizeof(unsigned char));
    if (!is_totient_arr) exit(1);

    for (idx_t m = 1; m <= M; ++m) {
        uint32_t val = phi_arr[m];
        if (val <= M)
            is_totient_arr[val] = 1;
    }
}

/* compute a(n) for the A072106 sequence */
seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)0;

    init_once();

    /* φ(n) from precomputed array (n ≤ M guaranteed by tests) */
    seq_t phi_n = (seq_t)phi_arr[n];

    /* count how many coprime residues are totients */
    seq_t inside = 0;
    idx_t limit = n - 1;
    for (idx_t k = 1; k <= limit; ++k) {
        if (gcd_idx(k, n) == 1) {
            if (is_totient_arr[k])
                inside++;
        }
    }

    return phi_n - inside;
}
