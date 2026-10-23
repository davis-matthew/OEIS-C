/*
 * A169372  Number of reduced words of length n in Coxeter group on 23
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^31 = I.
 * https://oeis.org/A169372
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A169372 – Number of reduced words of length n in the Coxeter group on
    23 generators S_i with relations S_i² = I and (S_i S_j)^31 = I.
    We model reduced words as length‑n strings over an alphabet of size
    23 without consecutive repeats and without an alternating run of
    length 31.  The automaton state is the current alternating run
    length l (2 ≤ l ≤ 30).  From a state l we can

        • extend the run (unique choice) → l+1,
        • or start a new run with a third letter (21 choices) → 2.

    The recurrence is iterated for each letter, giving an O(n) algorithm. */

seq_t a(idx_t n)
{
    /* Base cases -----------------------------------------------------*/
    if (n == 0) return (seq_t)1;                   /* 1      */
    if (n == 1) return (seq_t)23;                  /* 23     */
    if (n == 2) return (seq_t)23 * (seq_t)22;      /* 506    */

    /* State: l = alternating run length, 2 ≤ l ≤ 30                */
    const int MAX_L = 30;              /* maximum allowed alternating length  */
    seq_t curr[MAX_L + 1];
    seq_t next[MAX_L + 1];

    /* Initialise arrays to zero -------------------------------------*/
    for (int i = 0; i <= MAX_L; ++i) curr[i] = (seq_t)0, next[i] = (seq_t)0;

    /* Start with words of length 2 ---------------------------------*/
    curr[2] = (seq_t)23 * (seq_t)22;   /* 506 */

    /* Iterate over word lengths ----------------------------------*/
    for (idx_t len = 2; len < n; ++len)
    {
        /* Clear next state -----------------------------------------*/
        for (int i = 0; i <= MAX_L; ++i) next[i] = (seq_t)0;

        /* Transitions from each state l ----------------------------*/
        for (int l = 2; l <= MAX_L; ++l)
        {
            seq_t val = curr[l];
            if (val == (seq_t)0) continue;

            /* Extend alternating run (if allowed) ----------------*/
            if (l + 1 <= MAX_L) next[l + 1] += val;

            /* Start a new run: choose a third generator 21 ways ---*/
            next[2] += val * (seq_t)21;
        }

        /* If we have reached the target length, sum all states ----*/
        if (len + 1 == n)
        {
            seq_t sum = (seq_t)0;
            for (int l = 2; l <= MAX_L; ++l)
                sum += next[l];
            return sum;
        }

        /* Prepare for next iteration ------------------------------*/
        for (int i = 0; i <= MAX_L; ++i)
            curr[i] = next[i];
    }

    /* Unreachable for n ≥ 0 -------------------------------------*/
    return (seq_t)0;
}
