/*
 * A178356  Fibonacci numbers whose successive digits decrease by 1.
 * https://oeis.org/A178356
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Validated: full, pass, 8 terms through n = 8 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence a(n) consists of Fibonacci numbers (A000045)
   whose decimal representation has successive digits decreasing
   by exactly one.  Since only Fibonacci numbers < 9876543210
   are needed, the sequence is finite.  This implementation
   generates the Fibonacci numbers once, keeps the ones that
   satisfy the digit condition, and returns the nth term
   (zero‑based counting inside).  Subsequent calls reuse the
   static table.  If n is larger than the available terms,
   0 is returned. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Return true if x has successive digits that decrease by 1. */
static bool dec_by_one(seq_t x)
{
    if (x < 10)            /* single digit always ok */
        return true;
    seq_t next = x % 10;   /* least significant digit */
    x /= 10;
    while (x > 0) {
        seq_t cur = x % 10;
        if (cur != next + 1)          /* not decreasing by 1 */
            return false;
        next = cur;
        x /= 10;
    }
    return true;
}

seq_t a(idx_t n)
{
    /* static cache of good Fibonacci numbers */
    static seq_t *good = NULL;
    static size_t count = 0;

    if (!good) {
        /* Fibonacci numbers less than LLIMIT */
        const seq_t LLIMIT = 9876543210ULL;
        seq_t f0 = 0, f1 = 1;
        seq_t fib[64];
        size_t fibcnt = 0;

        fib[fibcnt++] = f0;
        fib[fibcnt++] = f1;
        while (1) {
            seq_t fn = f0 + f1;
            if (fn > LLIMIT)     /* stop before exceeding limit */
                break;
            fib[fibcnt++] = fn;
            f0 = f1;
            f1 = fn;
        }

        /* Allocate array for the good Fibonacci numbers */
        good = mem_new(seq_t, fibcnt);
        if (!good) return 0; /* allocation failure, return 0 */

        bool first = true;
        seq_t last = 0;
        for (size_t i = 0; i < fibcnt; ++i) {
            seq_t v = fib[i];
            if (!first && v == last)    /* skip duplicates (the two 1's) */
                continue;
            if (dec_by_one(v)) {
                good[count++] = v;
                last = v;
                first = false;
            }
        }
    }

    /* Sequence indices start at 1 */
    if (n == 0 || n-1 >= count)
        return 0;           /* undefined terms return 0 */
    return good[n-1];
}
