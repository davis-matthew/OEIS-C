/*
 * A145587  a(n) = number of numbers removed in each step of Eratosthenes's
 *          sieve for 2^9.
 * https://oeis.org/A145587
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, nonn
 * Formula:   a(n) = number of integers removed at step n of * Eratosthenes
 *            sieve for 2^9 (512). The sequence is obtained * by performing a
 *            classical sieve up to 512 and counting, for * each prime p, how
 *            many new composites are marked when * processing p. This works
 *            for any n ≥ 1; if n exceeds the * number of...
 * Range:     u8 n <= 8, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define N 512            /* 2^9 */
#define MAX_PRIMES 600   /* more than enough for N = 512 */

static seq_t *removal = NULL;     /* counts for each prime */
static idx_t removal_len = 0;      /* number of computed values */

/* One-time initialization: compute primes up to N and the
 * removal counts for each prime. */
static void init_counts(void)
{
    /* Prime sieve to list primes up to N. */
    static bool prime_flags[N + 1];
    static bool marked[N + 1];
    uint32_t primes[MAX_PRIMES];
    size_t pcnt = 0;

    memset(prime_flags, 1, sizeof(prime_flags));
    prime_flags[0] = prime_flags[1] = false;

    for (idx_t i = 2; i <= N; ++i) {
        if (prime_flags[i]) {
            primes[pcnt++] = (uint32_t)i;
            if (i * i <= N)
                for (idx_t j = i * i; j <= N; j += i)
                    prime_flags[(size_t)j] = false;
        }
    }

    /* Allocate array to store the removal counts. */
    removal = (seq_t *)malloc(pcnt * sizeof(seq_t));
    removal_len = pcnt;
    memset(marked, 0, sizeof(marked));

    /* For each prime, start marking at p^2 and count newly marked. */
    for (size_t pi = 0; pi < pcnt; ++pi) {
        idx_t p = (idx_t)primes[pi];
        seq_t cnt = 0;
        for (idx_t j = p * p; j <= N; j += p) {
            if (!marked[(size_t)j]) {
                marked[(size_t)j] = true;
                cnt++;
            }
        }
        removal[pi] = cnt;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* offset starts at 1 – 0 is undefined */
    if (removal == NULL) init_counts();
    if (n <= removal_len) return removal[n - 1];
    return 0;
}
