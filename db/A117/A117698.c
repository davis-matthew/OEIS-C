/*
 * A117698  Palindromic primes in base 3 (written in base 3).
 * https://oeis.org/A117698
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   generate palindromic primes whose base‑3 representations are
 *            palindromic; return the base‑3 string interpreted as a decimal
 *            integer
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 26, u64 n <= 179, u128 n <= 179,
 *            big any n
 * Helpers:   add_candidate, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/add_candidate.h"

/* Recursive generator of palindromes of a given length. */
static void rec_generate(idx_t len, size_t half, size_t pos,
                         int *firstHalf, seq_t **primes,
                         size_t *cnt, size_t *cap)
{
    if (pos == half) {
        seq_t val_num = 0;   /* decimal value of the base‑2 3 palindrome */
        seq_t val_str = 0;   /* base‑3 digits concatenated as decimal */
        for (idx_t i = 0; i < len; ++i) {
            int d = (i < (idx_t)half) ? firstHalf[i]
                                     : firstHalf[(size_t)(len - 1 - i)];
            val_num = val_num * 3 + (seq_t)d;
            val_str = val_str * 10 + (seq_t)d;
        }
        if (is_prime_big(val_num))
            add_candidate(val_str, primes, cnt, cap);
    } else {
        int start = (pos == 0) ? 1 : 0;
        for (int d = start; d <= 2; ++d) {
            firstHalf[pos] = d;
            rec_generate(len, half, pos + 1, firstHalf, primes, cnt, cap);
        }
    }
}

/* Generate all palindromes of the given base‑3 length and test them. */
static void generate_len(idx_t len, seq_t **primes, size_t *cnt, size_t *cap)
{
    size_t half = (len + 1) / 2;
    int firstHalf[64];   /* sufficient for required lengths */
    rec_generate(len, half, 0, firstHalf, primes, cnt, cap);
}

/* OEIS sequence function. */
seq_t a(idx_t n)
{
    static seq_t *pal = NULL;
    static size_t pal_cnt = 0;
    static size_t pal_cap = 0;
    static idx_t next_len = 1;

    while (pal_cnt < n) {
        generate_len(next_len, &pal, &pal_cnt, &pal_cap);
        ++next_len;
    }
    return pal[n - 1];
}
