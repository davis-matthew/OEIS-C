/*
 * A074915  Largest x such that the number of nonprimes (i.e., 1 and
 *          composites) in the reduced residue set (RRS(x)) of x equals n, or
 *          0 if there are no such numbers.
 * https://oeis.org/A074915
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 6770, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A074915:  a(n) = max{x | g(x)=n} ,  g(x)=φ(x)‑(π(x‑1)‑ω(x))  */
/*  φ(x)  : Euler’s totient.          π(x)  : number of primes ≤ x. */
/*  ω(x)  : number of distinct prime factors of x.  If no such x exists,
 *  a(n)=0.  This implementation precomputes φ, π and ω up to a fixed
 *  limit (2 000 000) and stores the maximal x for each g(x).  It works
 *  for seq_t and idx_t of any width; all auxiliary data use native
 *  machine types.  Complexity is O(L) for preprocessing where
 *  L=2 000 000.  Subsequent queries are O(1).  The chosen limit covers
 *  all n up to 10 000; for larger n the result may be 0.               */
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define MX 2000000UL          /* sufficient for the first 10 000 terms */
#define MAX_N 10000U         /* maximal n we answer for                 */

static uint32_t *phi_arr = NULL;
static uint8_t  *omega_arr = NULL;
static uint32_t *pi_arr = NULL;
static uint8_t  *prime_flag = NULL;
static uint32_t ans_arr[MAX_N + 1];

static void init_sieve(void)
{
    if (phi_arr != NULL) return;                 /* already done */

    /* Allocate arrays (sizes are in bytes) */
    phi_arr    = (uint32_t *)malloc((MX + 1) * sizeof(uint32_t));
    omega_arr  = (uint8_t  *)malloc((MX + 1) * sizeof(uint8_t));
    pi_arr     = (uint32_t *)malloc((MX + 1) * sizeof(uint32_t));
    prime_flag = (uint8_t  *)calloc((MX + 1), sizeof(uint8_t));

    /* linear sieve for φ and ω */
    uint32_t *prime_list = (uint32_t *)malloc((MX / 10 + 1) * sizeof(uint32_t));
    size_t   prime_cnt = 0;

    phi_arr[1] = 1;
    omega_arr[1] = 0;

    for (uint32_t i = 2; i <= MX; ++i) {
        if (!prime_flag[i]) {               /* i is prime */
            prime_list[prime_cnt++] = i;
            phi_arr[i] = i - 1;
            omega_arr[i] = 1;
        }
        for (size_t j = 0; j < prime_cnt; ++j) {
            uint32_t p = prime_list[j];
            uint64_t v = (uint64_t)i * p;
            if (v > MX) break;
            uint32_t n = (uint32_t)v;
            prime_flag[n] = 1;
            if (i % p == 0) {
                phi_arr[n] = phi_arr[i] * p;
                omega_arr[n] = omega_arr[i];
                break;
            } else {
                phi_arr[n] = phi_arr[i] * (p - 1);
                omega_arr[n] = omega_arr[i] + 1;
            }
        }
    }

    /* compute π[i] = number of primes ≤ i */
    pi_arr[0] = 0;
    for (uint32_t i = 1; i <= MX; ++i) {
        pi_arr[i] = pi_arr[i-1] + (!prime_flag[i] && i >= 2 ? 1 : 0);
    }

    /* initialise answer array */
    for (size_t i = 0; i <= MAX_N; ++i) ans_arr[i] = 0;

    /* fill answer table: for each x compute g(x) and keep maximal x */
    for (uint32_t x = 1; x <= MX; ++x) {
        uint32_t g = phi_arr[x] - (pi_arr[x-1] - omega_arr[x]); /* g(x) >= 0 */
        if (g == 0 || g > MAX_N) continue;
        if ((uint32_t)ans_arr[g] < x) ans_arr[g] = x;
    }

    /* free temporary prime list */
    free(prime_list);
}

seq_t a(idx_t n)
{
    static int initialized = 0;
    if (!initialized) {
        init_sieve();
        initialized = 1;
    }
    if (n < 1 || n > MAX_N) return 0;
    return ans_arr[(size_t)n];
}
