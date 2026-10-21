/*
 * A060832  a(n) = Sum_{k>0} floor(n/k!).
 * https://oeis.org/A060832
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k>0} floor(n/k!)
 * Range:     u8 n <= 149, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function computes A060832 for all n >= 0.  It uses only
 * integer arithmetic and works for any seq_t/idx_t size that the
 * build provides. */
seq_t a(idx_t n)
{
    /* Cast the index to the output type for arithmetic. */
    seq_t nn = (seq_t)n;
    seq_t fact = (seq_t)1;       /* k!  (starts with 1! = 1) */
    seq_t res  = (seq_t)0;       /* running sum */
    idx_t   k   = (idx_t)1;      /* current factorial index */

    /* Adding floor(nn / fact) while fact <= nn. */
    while (fact <= nn) {
        /* Add current term. */
        res += nn / fact;

        /* Prepare next factorial: now k will be the new index (k+1). */
        k++;

        /* Break if multiplying would overflow the bound: k! > nn. */
        if (fact > nn / (seq_t)k) {
            break;  /* Next factorial would exceed nn; loop exits. */
        }

        /* Compute next factorial. */
        fact *= (seq_t)k;
    }

    return res;
}
