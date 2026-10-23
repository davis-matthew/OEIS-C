/*
 * A032856  Numbers whose base-9 representation Sum_{i=0..m} d(i)*9^i has d(m)
 *          <= d(m-1) >= d(m-2) <= ...
 * https://oeis.org/A032856
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth element of A032856:
   Numbers whose base‑9 representation has digits d(m) <= d(m‑1) >= d(m‑2)
   <= d(m‑3) >= …  (alternating up/down starting with <=). */

#include <stdint.h>
#include <stdbool.h>

/* Recursive helper:
   n      – desired index (1‑based)
   len    – total number of base‑9 digits of the number being generated
   pos    – current position (0 = most significant digit)
   prev   – digit already chosen for position pos
   dir    – direction for the next comparison:
            0 → prev <= next, 1 → prev >= next
   val    – decimal value of digits placed so far
   cnt    – pointer to current count of generated numbers
   out    – pointer to store the answer once found
   Returns true if the nth number has been found and further
   recursion can be stopped. */
static bool gen_rec(idx_t n, int len, int pos,
                    uint8_t prev, int dir,
                    seq_t val,
                    idx_t *cnt, seq_t *out)
{
    if (pos == len - 1) {                /* last digit placed */
        (*cnt)++;                        /* this is a valid number */
        if (*cnt == n) {                 /* target reached */
            *out = val;
            return true;
        }
        return false;
    }

    uint8_t start, end;
    if (dir == 0) {                       /* prev <= next */
        start = prev;
        end   = 8;
    } else {                              /* prev >= next */
        start = 0;
        end   = prev;
    }

    for (uint8_t next = start; next <= end; ++next) {
        seq_t new_val = val * (seq_t)9 + (seq_t)next;
        if (gen_rec(n, len, pos + 1, next, dir ^ 1, new_val, cnt, out))
            return true;
    }
    return false;
}

/* Main function required by the harness */
seq_t a(idx_t n)
{
    idx_t  count = 0;
    seq_t  answer = 0;

    /* Numbers grow quickly, it is sufficient to explore lengths
       until the nth number is found. */
    for (int len = 1; ; ++len) {
        for (uint8_t msb = 1; msb <= 8; ++msb) {
            seq_t val = (seq_t)msb;
            if (gen_rec(n, len, 0, msb, 0, val, &count, &answer))
                return answer;
        }
    }
}
