/*
 * A119992  a(n) = n-th positive integer which is coprime to n!.
 * https://oeis.org/A119992
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 42, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = n-th positive integer coprime to n! (no simple closed form) */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* prime sieve limit – enough to generate all primes ≤ 10 000 */
#define PRIME_SIEVE_LIMIT 200000

/* ---------- prime generation ----------------------------------------- */
static uint32_t *global_primes = NULL;
static size_t    global_primes_cnt = 0;

static void
init_primes(void)
{
    if (global_primes != NULL)          /* already built */
        return;

    /* boolean array for the sieve */
    bool *comp = (bool*)calloc(PRIME_SIEVE_LIMIT + 1, sizeof(bool));
    if (!comp) exit(1);

    for (idx_t i = 2; (idx_t)i * i <= PRIME_SIEVE_LIMIT; ++i) {
        if (!comp[i]) {
            for (idx_t j = i * i; j <= PRIME_SIEVE_LIMIT; j += i)
                comp[j] = true;
        }
    }

    /* count primes */
    size_t cnt = 0;
    for (idx_t i = 2; i <= PRIME_SIEVE_LIMIT; ++i)
        if (!comp[i]) ++cnt;

    /* allocate and store primes */
    global_primes = (uint32_t*)malloc(cnt * sizeof(uint32_t));
    if (!global_primes) exit(1);
    size_t k = 0;
    for (idx_t i = 2; i <= PRIME_SIEVE_LIMIT; ++i)
        if (!comp[i]) global_primes[k++] = (uint32_t)i;

    global_primes_cnt = cnt;
    free(comp);
}

/* ---------- compute a(n) -------------------------------------------- */
static seq_t
compute_a_idx(idx_t n)
{
    seq_t candidate = (seq_t)1;
    idx_t count = 0;

    for (;; ++candidate) {
        bool coprime = true;
        /* test divisibility by every prime ≤ n */
        for (size_t pi = 0; pi < global_primes_cnt && global_primes[pi] <= n; ++pi) {
            seq_t p = (seq_t)global_primes[pi];
            if ((candidate % p) == 0) {
                coprime = false;
                break;
            }
        }

        if (coprime) {
            ++count;
            if (count == n)
                return candidate;
        }
    }
}

/* ---------- public interface ------------------------------------------ */
seq_t
a(idx_t n)
{
    if (n == 0)                 /* undefined for offset 1 – return 0 */
        return (seq_t)0;

    init_primes();               /* ensure primes are ready */
    return compute_a_idx(n);
}
