/*
 * A160402  Primes made up of all distinct digits except 0 and 1.
 * https://oeis.org/A160402
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, base
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 none, u64 n <= 3098, u128 n <= 3098,
 *            big any n
 * Helpers:   cmp_seq_t, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: Generate all 8‑digit permutations of {2..9}, test each for
   primality, sort the primes ascending, and return the nth such prime
   (OEIS A160402). */

#include <stdbool.h>
#include <stdlib.h>

#include "../utils/is_prime_big.h"  /* bool is_prime_big(seq_t n) */
#include "../utils/cmp_seq_t.h"     /* int cmp_seq_t(const void*,const void*) */

static void gen_rec(int depth, seq_t cur, bool *used,
                    const uint8_t digits[8], seq_t *prime_buf, size_t *cnt)
{
    if (depth == 8) {
        if (is_prime_big(cur))
            prime_buf[(*cnt)++] = cur;
        return;
    }
    for (int i = 0; i < 8; ++i) {
        if (!used[i]) {
            used[i] = true;
            gen_rec(depth + 1, cur * 10 + digits[i], used, digits,
                    prime_buf, cnt);
            used[i] = false;
        }
    }
}

/* External linkage as required. */
seq_t a(idx_t n)
{
    /* One‑time initialisation (static local data). */
    static bool initialized = false;
    static seq_t primes[40320];
    static size_t prime_cnt = 0;

    if (!initialized) {
        bool used[8] = {0};
        const uint8_t digits[8] = {2,3,4,5,6,7,8,9};

        /* Generate all permutations and keep only the primes. */
        gen_rec(0, 0, used, digits, primes, &prime_cnt);

        /* Sort the primes in ascending order. */
        qsort(primes, prime_cnt, sizeof(seq_t), cmp_seq_t);

        initialized = true;
    }

    /* The sequence is finite; return 0 for out‑of‑range indices. */
    if (n == 0 || n > prime_cnt)
        return 0;

    return primes[(size_t)(n - 1)];   /* a(1) is the first stored prime */
}
