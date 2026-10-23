/*
 * A178828  Partial sums of floor(3^n/10)/2.
 * https://oeis.org/A178828
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 22, u64 n <= 42, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Formula (OEIS A178828):  a(n) = (floor(3^1/10)+floor(3^2/10)+...+floor(3^n/10))/2
    Recurrence: a(1)=0, a(2)=0, a(3)=1, a(4)=5,
    a(n)=a(n-4)+2·3^(n-3)−1  (n>4)
    The function computes terms up to at least n=10 000 in O(n) arithmetic time.
    It uses a static cache to handle any order of calls.
*/
seq_t a(idx_t n)
{
    /* The sequence is defined only for n >= 1.  For n < 1 we return 0. */
    if (n < 1) return (seq_t)0;

    /* 10001 entries allow computation for n up to 10 000 (index 0 unused). */
    static seq_t cache[10001];
    static idx_t known = 0;      /* highest index that is already computed */
    static seq_t power3 = 0;    /* 3^(i-3) for the last computed index i >=5 */
    static idx_t last_i = 0;    /* last index for which power3 holds 3^(i-3) */

    /* ------- Initialize base cases (first time only) ------- */
    if (known == 0)
    {
        cache[0] = 0;          /* unused, but keeps indices 1..4 valid */

        cache[1] = 0;          /* a(1) */
        cache[2] = 0;          /* a(2) */
        cache[3] = 1;          /* a(3) */
        cache[4] = 5;          /* a(4) */

        known   = 4;
        last_i  = 4;
        power3  = 0;           /* unused until n >= 5 */
    }

    /* ------- Extend cache up to the requested index ------- */
    for (idx_t i = known + 1; i <= n; ++i)
    {
        if (i <= 4)
        {
            /* All values for i=1..4 are already filled in the initial block. */
            continue;
        }

        /* Compute 3^(i-3):  if i==5 set to 9, else multiply previous by 3. */
        if (i == 5)
            power3 = (seq_t)9;        /* 3^(5-3) = 3^2 = 9 */
        else
            power3 = power3 * (seq_t)3;

        /* Apply the recurrence: a(i) = a(i-4) + 2*3^(i-3) - 1 */
        cache[i] = cache[i - 4] + power3 + power3 - (seq_t)1;

        /* Update state for the next iteration */
        known  = i;
        last_i = i;
    }

    return cache[n];
}
