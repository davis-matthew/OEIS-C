/*
 * A023500  a(n) = b(n) + d(n), where b(n) = (n-th Fibonacci number > 1) and
 *          d(n) = (n-th number that is 1, 2, or 3, or is not a Lucas number).
 * https://oeis.org/A023500
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 21, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute  a(n) = b(n) + d(n)   with
 *   b(n) = (n-th Fibonacci number > 1) = Fib(n+3)
 *   d(n) = (n-th natural number that is 1, 2, or 3, or is not a Lucas number)
 * The function evaluates the sum for any n ≥ 0. It uses the supplied
 *  utility fib(idx_t) to obtain Fibonacci numbers and enumerates natural
 *  numbers to obtain d(n). Performance is linear in n, well below the
 *  required 2 s per call for n up to ~10 000.  */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "../utils/fib.h"

/* static storage for computed Lucas numbers */
static uint64_t *lucas = NULL;
static size_t   lucas_len = 0;
static size_t   lucas_cap = 0;

/* initialise the Lucas list with the first two terms 2, 1 */
static void lucas_init(void)
{
    if (lucas == NULL) {
        lucas_cap = 2;
        lucas = (uint64_t *)malloc(lucas_cap * sizeof(uint64_t));
        lucas[0] = 2;
        lucas[1] = 1;
        lucas_len = 2;
    }
}

/* ensure that all Lucas numbers up to at least `upto` have been generated */
static void lucas_ensure(uint64_t upto)
{
    if (lucas == NULL) lucas_init();

    while (lucas_len > 0 && lucas[lucas_len - 1] < upto) {
        uint64_t next = lucas[lucas_len - 2] + lucas[lucas_len - 1];

        if (lucas_len == lucas_cap) {
            lucas_cap = lucas_cap * 2 ? lucas_cap * 2 : 4;
            lucas = (uint64_t *)realloc(lucas, lucas_cap * sizeof(uint64_t));
        }
        lucas[lucas_len++] = next;
    }
}

/* helper: test whether a number is a Lucas number (and at least 4) */
static bool is_lucas_ge4(uint64_t x)
{
    if (x < 4) return false;   /* 1,2,3 are handled separately */
    for (size_t i = 0; i < lucas_len; ++i) {
        if (lucas[i] == x) return true;
    }
    return false;
}

/* main sequence function */
seq_t a(idx_t n)
{
    /* part b(n): nth Fibonacci number > 1  ==> Fib(n+3) */
    seq_t fib_part = fib(n + 3);

    /* part d(n): nth natural number that is 1,2,3 or not a Lucas number */
    idx_t target = n;            /* 0‑based index */
    idx_t count  = 0;            /* how many valid numbers found so far   */
    uint64_t i   = 1;            /* current candidate number               */

    lucas_init();                /* ensure Lucas table exists              */

    while (true) {
        lucas_ensure(i);          /* generate Lucas numbers up to i         */

        bool include;
        if (i <= 3) {
            include = true;        /* 1,2,3 are always included             */
        } else {
            include = !is_lucas_ge4(i);
        }

        if (include) {
            if (count == target) {
                seq_t d_part = (seq_t)i;
                return fib_part + d_part;
            }
            ++count;
        }
        ++i;
    }
}
