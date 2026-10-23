/*
 * A106901  Primes of the form 3x^2+3xy+5y^2, with x and y nonnegative.
 * https://oeis.org/A106901
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime of the form 3*x^2 + 3*x*y + 5*y^2 (x,y ≥ 0) *
 *            (computed by exhaustive search)
 * Range:     u8 n <= 11, u16 n <= 1224, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   cmp_seq_t, isPrime64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/isPrime64.h"
#include "../utils/cmp_seq_t.h"

#define MAX_PRIMES 30000          /* enough for n up to ~10000 */

static seq_t prime_list[MAX_PRIMES];
static size_t prime_count = 0;
static uint64_t current_limit = 1;   /* lower bound for next search */

/* Generate all prime values of the form 3x^2+3xy+5y^2 ≤ limit.
 * The result is stored in prime_list (sorted, unique). */
static void generate_primes_up_to(uint64_t limit)
{
    size_t count = 0;

    for (uint64_t y = 0; ; ++y) {
        bool any_for_y = false;
        for (uint64_t x = 0; ; ++x) {
            unsigned __int128 val =
                (unsigned __int128)3 * x * x +
                (unsigned __int128)3 * x * y +
                (unsigned __int128)5 * y * y;
            if (val > limit)
                break;
            uint64_t v = (uint64_t)val;
            if (v >= 3 && isPrime64(v)) {
                if (count < MAX_PRIMES)
                    prime_list[count++] = (seq_t)v;
            }
            any_for_y = true;
        }
        if (!any_for_y)      /* even x=0 gives value > limit */
            break;
    }

    /* sort and remove duplicates */
    qsort(prime_list, count, sizeof(seq_t), cmp_seq_t);
    size_t uniq = 0;
    for (size_t i = 0; i < count; ++i) {
        if (uniq == 0 || prime_list[i] != prime_list[uniq - 1]) {
            prime_list[uniq++] = prime_list[i];
        }
    }
    prime_count = uniq;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)               /* undefined for offset 1 */
        return 0;

    /* grow search limit until we have enough primes */
    while (prime_count < (size_t)n) {
        if (current_limit < 2)
            current_limit = 2;
        else {
            /* avoid overflow */
            if (current_limit > (uint64_t)(-1) / 2)
                current_limit = (uint64_t)(-1);
            else
                current_limit <<= 1;
        }
        generate_primes_up_to(current_limit);
    }

    return prime_list[n - 1];
}
