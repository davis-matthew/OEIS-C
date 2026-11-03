/*
 * A101608  Solution to Tower of Hanoi puzzle encoded in pairs with the moves
 *          (1,2),(2,3),(3,1),(2,1),(3,2),(1,3). The disks are moved from peg
 *          1 to 2. For a tower of k disks use the first 2^k-1 number pairs.
 * https://oeis.org/A101608
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   Recurrence: a(1)=1, a(2)=2, a(3)=1, a(4)=3. For n≥5: a(4k+1) =
 *            (k mod 3) + 1, a(4k+2) = ((k+1) mod 3) + 1, a(4k+3) =
 *            f(a(2k+1)), a(4k+4) = f(a(2k+2)), where f(1)=1, f(2)=3, f(3)=2
 * Range:     u8 n <= 102, u16 n <= 102, u32 n <= 102, u64 n <= 102,
 *            u128 n <= 102, big any n
 * Validated: partial, pass, 102 terms through n = 102 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline seq_t f_seq(seq_t v)
{
    return (v == 1) ? (seq_t)1
         : (v == 2) ? (seq_t)3
                      : (seq_t)2;
}

static seq_t a_rec(idx_t n)
{
    if (n <= 4) {
        switch (n) {
            case 1: return (seq_t)1;
            case 2: return (seq_t)2;
            case 3: return (seq_t)1;
            case 4: return (seq_t)3;
        }
    }
    switch (n & 3U) {                      /* n % 4 */
        case 1U: {                         /* a(4k+1) */
            idx_t k = (n - 1) >> 2;        /* (n-1)/4 */
            return (seq_t)((k % 3) + 1);
        }
        case 2U: {                         /* a(4k+2) */
            idx_t k = (n - 2) >> 2;        /* (n-2)/4 */
            return (seq_t)(((k + 1) % 3) + 1);
        }
        case 3U: {                         /* a(4k+3) */
            idx_t k = (n - 3) >> 2;        /* (n-3)/4 */
            return f_seq(a_rec((idx_t)((seq_t)2 * k + (seq_t)1)));
        }
        case 0U: {                         /* a(4k+4) */
            idx_t k = (n - 4) >> 2;        /* (n-4)/4 */
            return f_seq(a_rec((idx_t)((seq_t)2 * k + (seq_t)2)));
        }
        default: return (seq_t)0;          /* unreachable */
    }
}

seq_t a(idx_t n)
{
    return a_rec(n);
}
