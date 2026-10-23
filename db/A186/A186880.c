/*
 * A186880  Number of (n+2) X 10 0..3 arrays with every 3 X 3 subblock
 *          commuting with each horizontal and vertical neighbor 3 X 3
 *          subblock.
 * https://oeis.org/A186880
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 25, u64 n <= 68, u128 n <= 155,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 2*a(n-1) - a(n-2) + 3*a(n-3) - 6*a(n-4)
                      + 3*a(n-5) + 3*a(n-6) - 3*a(n-7)   for n > 16
   Initial values:
     a(1)  = 322910
     a(2)  = 25381
     a(3)  = 46624
     a(4)  = 148048
     a(5)  = 310528
     a(6)  = 414184
     a(7)  = 571984
     a(8)  = 801208
     a(9)  = 1130584
     a(10) = 1842208
     a(11) = 3014488
     a(12) = 4813744
     a(13) = 8147608
     a(14) = 13457800
     a(15) = 21637048
     a(16) = 37154419 */
 
seq_t a(idx_t n)
{
    /* Array of the first 16 terms, 1‑based index */
    static const seq_t first[17] = {
        0ULL,          /* unused, index 0 */
        322910ULL,
        25381ULL,
        46624ULL,
        148048ULL,
        310528ULL,
        414184ULL,
        571984ULL,
        801208ULL,
        1130584ULL,
        1842208ULL,
        3014488ULL,
        4813744ULL,
        8147608ULL,
        13457800ULL,
        21637048ULL,
        37154419ULL
    };
    
    if (n <= 16)
        return first[(idx_t)n];
    
    /* Sliding window of the 7 most recent values:
       t[0] = a[i-1], t[1] = a[i-2], ..., t[6] = a[i-7]   */
    seq_t t[7];
    idx_t i;
    
    /* initialise window for i = 17 (needs a[16]..a[10]) */
    t[0] = first[16];
    t[1] = first[15];
    t[2] = first[14];
    t[3] = first[13];
    t[4] = first[12];
    t[5] = first[11];
    t[6] = first[10];
    
    for (i = 17; i <= n; ++i)
    {
        seq_t next = 0;
        next = 2 * t[0]            /* a[i-1]   */
             - t[1]                /* a[i-2]   */
             + 3 * t[2]            /* a[i-3]   */
             - 6 * t[3]            /* a[i-4]   */
             + 3 * t[4]            /* a[i-5]   */
             + 3 * t[5]            /* a[i-6]   */
             - 3 * t[6];           /* a[i-7]   */
        
        /* shift window right and insert the new value at position 0 */
        t[6] = t[5];
        t[5] = t[4];
        t[4] = t[3];
        t[3] = t[2];
        t[2] = t[1];
        t[1] = t[0];
        t[0] = next;
    }
    
    return t[0];   /* This holds a[n] after the loop */
}
