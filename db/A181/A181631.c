/*
 * A181631  Triangle by rows, number of leading 1's in the maximal Fibonacci
 *          representation (A104326).
 * https://oeis.org/A181631
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS formula: a(n) = A090996(A003754(n+1)) */
/* This implementation builds the triangle described in A181631
   using the recurrence:
        row r has length L(r) = F_{r+1} where
            F(1)=1, F(2)=2, F(k)=F(k-1)+F(k-2)  (Fibonacci starting 1,2)
        the first P(r) entries of row r are 1, where
            P(r) = 1    for r = 1 or r = 2
            P(r) = L(r-2) for r >= 3
        the remaining entries are previous_row_entry + 1.
   For an index n (1‑based) we repeatedly: if the position within
   its row is in the prefix, the value is 1; otherwise we add 1
   and move to the entry that lies in the previous row at the same
   offset.  The number of times we traverse from a longer row to a
   shorter row gives a(n).  This is exactly the OEIS definition
   of A181631.  The routine is O(log n) for any 64‑bit index.
*/

#include <stddef.h>
#include <stdbool.h>

static bool _init_done = false;
static idx_t _fib_len[120];   /* Fibonacci lengths L(r) = F_{r+1} (1,2,3,5,...) */
static idx_t _cumul[120];    /* cumulative prefix sums of _fib_len */

/* Initialize tables once */
static void _init_fib(void)
{
    if (_init_done) return;
    _fib_len[0] = 0;       /* unused */
    _fib_len[1] = 1;       /* L(1) = 1 */
    _fib_len[2] = 2;       /* L(2) = 2 */
    idx_t r = 3, last_a = 2, last_b = 1;   /* fib values: last_b=F_{r-2}, last_a=F_{r-1} */
    while (r < 120)
    {
        idx_t next = last_a + last_b;            /* F_{r-1}+F_{r-2} */
        if (next < last_a) break;                /* overflow, stop */
        _fib_len[r] = next;                      /* L(r) = next */
        last_b = last_a;
        last_a = next;
        ++r;
    }
    _cumul[0] = 0;
    for (r = 1; r < 120; ++r)
    {
        if (_fib_len[r] == 0) break;
        if (_cumul[r-1] > (idx_t)-1 - _fib_len[r]) _cumul[r] = (idx_t)-1;
        else _cumul[r] = _cumul[r-1] + _fib_len[r];
    }
    _init_done = true;
}

/* Prefix length used in a row */
static inline idx_t _prefix_len(idx_t r)
{
    if (r <= 2) return 1;
    return _fib_len[r-2];
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;     /* safety, though n>=1 per spec */
    _init_fib();

    idx_t target = n;   /* 1‑based index */
    seq_t value  = 0;

    while (true)
    {
        /* find row r such that _cumul[r] >= target */
        idx_t r = 1;
        while (_cumul[r] < target) ++r;

        idx_t start   = _cumul[r-1] + 1;        /* first index in this row (1‑based) */
        idx_t pos     = target - start + 1;      /* position within the row (1‑based) */
        idx_t pref    = _prefix_len(r);

        if (pos <= pref)      /* inside the leading block of 1's */
        {
            value += 1;
            break;
        }
        else
        {
            value += 1;                             /* +1 for the +1 rule */
            target -= _fib_len[r];                  /* move to the corresponding element
                                                        in the previous row */
        }
    }

    return value;
}
