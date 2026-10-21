/*
 * A055042  Numbers of the form 2^(2i+1)*(8*j+5).
 * https://oeis.org/A055042
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 3294, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers of the form 2^(2i+1)*(8*j+5). */

/* Helper: count how many numbers of that form are ≤ X. */
static seq_t count_leq(seq_t X)
{
    seq_t cnt = 0;
    /* Loop over odd exponents e = 1,3,5,... */
    for (int exp = 1; exp < 64; exp += 2)
    {
        seq_t pow2 = (seq_t)1 << exp;          /* 2^exp */
        if (pow2 > X / (seq_t)5) break;         /* base would exceed X */
        seq_t base = pow2 * (seq_t)5;            /* 5*2^exp */
        seq_t step = pow2 * (seq_t)8;            /* 8*2^exp = 2^(exp+3) */
        if (X < base) continue;                 /* should not happen */
        seq_t add = 1 + (X - base) / step;       /* number of terms from this e */
        cnt += add;
    }
    return cnt;
}

/* Main sequence function.  a(n) is the nth smallest integer of the form
   2^(2i+1)*(8*j+5), with offset 1. */
seq_t a(idx_t n)
{
    seq_t target = (seq_t)n;         /* number of terms to find */
    seq_t low = 1;
    seq_t high = target * (seq_t)12 + 100;  /* rough upper bound */

    /* Ensure high is large enough */
    while (count_leq(high) < target)
        high = high * 2;

    /* Binary search for the smallest X with count_leq(X) ≥ target */
    while (low < high)
    {
        seq_t mid = low + (high - low) / 2;
        if (count_leq(mid) >= target)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}
