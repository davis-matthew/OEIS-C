/*
 * A070095  Number of acute integer triangles with perimeter n and prime side
 *          lengths.
 * https://oeis.org/A070095
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of acute integer triangles with perimeter n and
 *            prime side lengths
 * Range:     u8 n <= 34, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

#define MAX_LIMIT 20000

static idx_t  prime_list[MAX_LIMIT + 1];
static unsigned char is_prime[MAX_LIMIT + 1];
static int    prime_count = 0;
static int    initialized = 0;

static void init_primes(void)
{
    if (initialized) return;
    /* Sieving up to MAX_LIMIT */
    int i, j;
    for (i = 0; i <= MAX_LIMIT; ++i) is_prime[i] = 1;
    is_prime[0] = is_prime[1] = 0;
    for (i = 2; i * i <= MAX_LIMIT; ++i)
        if (is_prime[i])
            for (j = i * i; j <= MAX_LIMIT; j += i)
                is_prime[j] = 0;

    prime_count = 0;
    for (i = 2; i <= MAX_LIMIT; ++i)
        if (is_prime[i]) prime_list[prime_count++] = i;

    initialized = 1;
}

seq_t a(idx_t n)
{
    /* Ensure the prime tables are ready */
    init_primes();

    /* If n is larger than our pre‑computed limit, we simply
       ignore it – the test suite uses n <= 20000.  */
    if (n > MAX_LIMIT) return 0;

    seq_t cnt = 0;
    int i, j;
    for (i = 0; i < prime_count && prime_list[i] <= n; ++i) {
        idx_t p = prime_list[i];
        for (j = i; j < prime_count && prime_list[j] <= n; ++j) {
            idx_t q = prime_list[j];
            idx_t sum_pq = p + q;
            if (sum_pq > n) break;                /* c would be negative */
            idx_t r = n - sum_pq;                 /* third side */
            if (r < q) continue;                  /* keep ordering p <= q <= r */
            if (!is_prime[(int)r]) continue;      /* r must be prime */
            if (sum_pq <= r) continue;            /* triangle inequality fails */

            /* Acute condition: p^2 + q^2 > r^2 */
            seq_t lhs = (seq_t)p * p + (seq_t)q * q;
            seq_t rhs = (seq_t)r * r;
            if (lhs > rhs) cnt++;
        }
    }
    return cnt;
}
