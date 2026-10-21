/*
 * A097422  Sum{k=1 to n} H(k) k!, where H(k) = sum{j=1 to k} 1/j.
 * https://oeis.org/A097422
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Σ_{k=1..n} H(k) * k! = Σ_{k=1..n} s(k+1,2) where H(k) =
 *            Σ_{j=1..k} 1/j and s(n,2) are unsigned Stirling numbers of the
 *            first kind. The recurrence used is S_k = k * S_{k-1} + (k-1)!
 *            (S_k = s(k+1,2)) with S_0 = 0 and fact = (k-1)!. The function
 *            returns the sum of S_k for k = 1..n
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    seq_t sum   = 0;   // running sum of terms
    seq_t S     = 0;   // S_{k-1} : s(k,2) for previous k
    seq_t fact  = 1;   // fact_{k-1} : (k-1)!  , fact for k=0 is 0! = 1

    for (idx_t k = 1; k <= n; ++k)
    {
        // term = s(k+1,2) = k * S + (k-1)!
        seq_t term = (seq_t)k * S + fact;
        sum += term;

        // update for next iteration
        S    = term;
        fact = fact * (seq_t)k;   // fact = k! for next k
    }
    return sum;
}
