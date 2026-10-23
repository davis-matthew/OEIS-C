/*
 * A164686  Number of reduced words of length n in Coxeter group on 42
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^7 = I.
 * https://oeis.org/A164686
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A164686: Number of reduced words of length n in the Coxeter
   group with 42 generators S_i, relations S_i^2 = (S_i S_j)^7 = I.
   Recurrence derived from the rational generating function:
     a(n) = 40*a(n-1) + 40*a(n-2) + 40*a(n-3) + 40*a(n-4) + 40*a(n-5)
            + 40*a(n-6) - 820*a(n-7) + b(n)
   where b(n) = 1,2,2,2,2,2,2,1 for n = 0..7 and 0 otherwise.  */
/* External linkage */
seq_t a(idx_t n)
{
    /* Precomputed numerator coefficients */
    static const seq_t b[8] = {1,2,2,2,2,2,2,1};

    /* Cache up to a fixed maximum (covers n up to about 20000) */
    static seq_t cache[20001];
    static idx_t next = 0;        /* next index to compute */

    /* Compute values up to n lazily */
    if (n >= next) {
        for (idx_t i = next; i <= n; ++i) {
            seq_t val = 0;
            if (i >= 1) val += 40 * cache[i-1];
            if (i >= 2) val += 40 * cache[i-2];
            if (i >= 3) val += 40 * cache[i-3];
            if (i >= 4) val += 40 * cache[i-4];
            if (i >= 5) val += 40 * cache[i-5];
            if (i >= 6) val += 40 * cache[i-6];
            if (i >= 7) val -= 820 * cache[i-7];
            if (i < 8)  val += b[i];
            cache[i] = val;
        }
        next = n + 1;
    }
    return cache[n];
}
