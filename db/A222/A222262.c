/*
 * A222262  Divisors of Descarte's 198585576189.
 * https://oeis.org/A222262
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 167, u32 n <= 476, u64 n <= 486,
 *            u128 n <= 486, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A222262: a(n) is the n‑th smallest divisor of 198585576189 */
#include <stdlib.h>
#include <stdint.h>

/* Prime factorisation of 198585576189 = 3^2 * 7^2 * 11^2 * 13^2 * 19^2 * 61^1 */
#define NUM_PRIMES   6
#define NUM_DIVISORS 486

static const uint64_t PRIME_LIST[NUM_PRIMES] = {3ULL, 7ULL, 11ULL, 13ULL, 19ULL, 61ULL};
static const int EXP_LIST[NUM_PRIMES]      = {2,    2,    2,    2,    2,    1};

static seq_t divisors[NUM_DIVISORS];
static size_t divisor_count = 0;

/* qsort comparison for uint64_t */
static int cmp_u64(const void *a, const void *b)
{
    const uint64_t *x = (const uint64_t *)a;
    const uint64_t *y = (const uint64_t *)b;
    return (*x < *y) ? -1 : (*x > *y) ? 1 : 0;
}

/* Recursive generation of all divisors, stored in buffer 'buf' */
static size_t gen_index = 0;
static void gen_divs_rec(int pos, uint64_t cur, uint64_t *buf)
{
    if (pos == NUM_PRIMES) {
        buf[gen_index++] = cur;
        return;
    }
    uint64_t t = cur;
    for (int e = 0; e <= EXP_LIST[pos]; ++e) {
        gen_divs_rec(pos + 1, t, buf);
        t *= PRIME_LIST[pos];
    }
}

/* One‑time initialisation: generate, sort, and store divisors in seq_t */
static void init_divs(void)
{
    uint64_t buf[NUM_DIVISORS];
    gen_index = 0;
    gen_divs_rec(0, 1ULL, buf);

    qsort(buf, gen_index, sizeof(uint64_t), cmp_u64);

    divisor_count = gen_index;
    for (size_t i = 0; i < divisor_count; ++i) {
        divisors[i] = (seq_t)buf[i];
    }
}

seq_t a(idx_t n)
{
    static int inited = 0;
    if (!inited) {
        init_divs();
        inited = 1;
    }
    if (n == 0) return (seq_t)0;
    if ((size_t)(n - 1) < divisor_count) return divisors[n - 1];
    return (seq_t)0;          /* undefined for n > number of divisors */
}
