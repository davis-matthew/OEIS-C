/*
 * A057520  a(n) = A014486(n)/2. In binary expansion there is one more 1 than
 *          0 and reading from the left (the most significant bit) to right,
 *          the number of 0's never exceed the number of 1's.
 * https://oeis.org/A057520
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Computation of OEIS A057520: a(n) = A014486(n)/2
 * A014486(n) lists even numbers whose binary representation has a
 * balanced number of 1's and 0's, with the property that in the
 * left‑to‑right scan the number of 0's never exceeds the number of 1's.
 * The numbers are enumerated in increasing order.  This implementation
 * generates the n-th Dyck word of even length (Catalan enumeration)
 * and returns half of its value.
 */

#include <stdint.h>

static inline unsigned __int128 binom128(idx_t N, idx_t K)
{
    if (K > N) return 0;
    if (K > N - K) K = N - K;
    unsigned __int128 result = 1;
    for (idx_t i = 1; i <= K; ++i)
        result = result * (N - K + i) / i;
    return result;
}

static inline unsigned __int128 catalan128(idx_t M)
{
    unsigned __int128 val = binom128(2 * M, M);
    return val / (M + 1);
}

/* Count of Dyck completions for given remaining bits and balance.
   The result is capped at limit + 1 (so 0 <= result <= limit+1). */
static inline unsigned __int128 count_dyck128(idx_t rem, idx_t bal,
                                             unsigned __int128 limit)
{
    if (bal > rem) return 0;
    if ((rem - bal) & 1) return 0;
    idx_t t = (rem + bal) / 2;
    unsigned __int128 term1 = binom128(rem, t);
    unsigned __int128 term2 = binom128(rem, t + 1);
    unsigned __int128 cnt = term1 - term2;
    if (cnt > limit) return limit + 1;
    return cnt;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* Find the block (even length) that contains the n-th term. */
    idx_t sumPrev = 0;
    idx_t m = 0;                /* length = 2*m bits */
    for (m = 1; ; ++m)
    {
        unsigned __int128 cnt = catalan128(m);
        unsigned __int128 sum = (unsigned __int128)sumPrev + cnt;
        if (sum >= n) break;    /* block found */
        sumPrev = (idx_t)sum;
    }

    /* Index inside this block (0‑based). */
    idx_t k = n - sumPrev - 1;

    /* Build the Dyck word of length 2*m bits. */
    idx_t rem = 2 * m - 1;      /* bits still to place after leading 1 */
    int bal = 1;                /* current balance (ones minus zeros) */
    seq_t val = (seq_t)1;       /* leading 1 */

    while (rem > 0)
    {
        if (bal == 0)
        {
            /* Must place '1' to keep balance non‑negative. */
            val = (val << 1) | 1;
            ++bal;
        }
        else
        {
            /* Count completions if we put a '0' next. */
            unsigned __int128 cnt0 = count_dyck128(rem - 1, bal - 1,
                                                   (unsigned __int128)k);
            if ((unsigned __int128)k < cnt0)
            {
                /* 0 block contains the desired word. */
                val = val << 1;   /* bit 0 */
                --bal;
            }
            else
            {
                /* Skip the 0 block and choose 1. */
                val = (val << 1) | 1;
                k -= (idx_t)cnt0;
                ++bal;
            }
        }
        --rem;
    }

    /* Divide by 2 (last bit is always 0). */
    return val >> 1;
}
