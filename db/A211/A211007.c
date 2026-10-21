/*
 * A211007  Surface area of the first n faces of the structure mentioned in
 *          A211006.
 * https://oeis.org/A211007
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = T_k where k = 1, if n = 1 k = p_{n/2}, if n is even k =
 *            p_{(n+1)/2} – 1, if n is odd (n > 1) p_j denotes the j‑th odd
 *            prime (3, 5, 7, 11, 13, …). T_k = k*(k+1)/2 is the k‑th
 *            triangular number
 * Range:     u8 n <= 15, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Internal storage for the first few odd primes. */
static uint32_t g_primes[200000];
static size_t   g_prime_cnt  = 0;
static bool     g_inited     = false;

/* Build a list of odd primes up to LIMIT inclusive.
   The limit is chosen well above the required amount for n ≤ 10000
   (≈5000 odd primes). */
static void init_primes(void)
{
    if (g_inited) return;
    const int LIMIT = 200000;                /* covers >17000 odd primes */
    bool *is_prime = (bool *)malloc((LIMIT + 1) * sizeof(bool));
    if (!is_prime) {                         /* allocation failed → abort */
        g_inited = true;                     /* no primes available */
        return;
    }

    for (int i = 0; i <= LIMIT; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;

    for (int i = 2; i * i <= LIMIT; ++i) {
        if (is_prime[i]) {
            for (int j = i * i; j <= LIMIT; j += i)
                is_prime[j] = false;
        }
    }

    for (int i = 3; i <= LIMIT; i += 2) {
        if (is_prime[i]) {
            g_primes[g_prime_cnt++] = (uint32_t)i;
        }
    }
    free(is_prime);
    g_inited = true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* offset is 1 – behaviour below offset is irrelevant */
    if (n == 1) return 1;            /* a(1) = 1 */

    init_primes();                    /* ensure primes are ready */

    /* Determine the appropriate odd prime index (1‑based). */
    idx_t prime_index;
    seq_t k;
    if ((n & 1) == 0) {              /* even n → k = p_{n/2} */
        prime_index = n / 2;
        /* convert to 0‑based index for g_primes array */
        k = (seq_t)g_primes[prime_index - 1];
    } else {                          /* odd n ≥ 3 → k = p_{(n+1)/2} – 1 */
        prime_index = (n + 1) / 2;
        k = (seq_t)g_primes[prime_index - 1] - 1;
    }

    /* Return the triangular number k*(k+1)/2 using only integer ops. */
    return k * (k + 1) / 2;
}
